#include <chrono>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <iostream>
#include <thread>

#include "component/component.h"
#include "engine/renderer/shader.h"
#include "engine/window/window_factory.h"

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

const double MS_PER_UPDATE = 1000.0 / 50.0;
const double MAX_FPS = 60.0;
const double FRAME_TARGET = 1.0 / MAX_FPS;

unsigned int init_mesh(mesh_component& mesh) {
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float),
                 mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.indices.size() * sizeof(unsigned int),
                 mesh.indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);

    glBindVertexArray(0);
    return VAO;
}

void render_mesh(unsigned int VAO, mesh_component& mesh,
                 transform_component& transform, color_component& color,
                 shader& shader) {
    shader.set_mat4("model", transform.transform);
    shader.set_vec4("u_color", color.color);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int main() {
    context_window* window = create_window(SCR_WIDTH, SCR_HEIGHT, "bitghetti");

    // Main rendering loop
    // -------------------

    double prev_time = window->get_time(), fps_timer = prev_time;
    double lag = 0.0;
    int frames = 0;
    int pframes = 0;
    // int updates = 0;

    shader shader("shaders/test.vert", "shaders/test.frag");

    // Test Cube - only vertices for now
    // ---------------------------------
    // clang-format off
    // float vertices[] = {// Front face
    //     -0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f, -0.5f,  0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f
    // };

    // test square
    std::vector<float> vertices = {
        // positions
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    std::vector<unsigned int> indices = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    // clang-format on

    mesh_component mesh(vertices, indices);
    color_component color(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
    transform_component transform(glm::mat4(1.0f));

    unsigned int VAO = init_mesh(mesh);

    // mouse picking framebuffer
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    while (window->is_open()) {
        double curr_time = window->get_time();
        double delta_time = curr_time - prev_time;
        prev_time = curr_time;
        lag += delta_time;

        window->process_input();

        while (lag >= MS_PER_UPDATE) {
            // Update game simulation

            lag -= MS_PER_UPDATE;
        }

        // Render here (lag / MS_PER_UPDATE)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // create transformations
        glm::mat4 model = glm::mat4(
            1.0f);  // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        transform.transform = glm::rotate(model, (float)glfwGetTime(),
                                          glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f),
                                      (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                      0.1f, 100.0f);
        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        // render box
        render_mesh(VAO, mesh, transform, color, shader);

        window->update();
        frames++;

        // Update FPS
        if (window->get_time() - fps_timer > 1.0) {
            fps_timer += 1.0;
            pframes = frames;
            frames = 0;
        }

        // Print FPS and Cursor Pos
        // auto [xpos, ypos] = window->get_cursor_pos();
        // std::cout << "\r" << std::setfill(' ') << std::setw(10) << std::left
        //           << "FPS: " << pframes << std::setw(10) << "\txpos: " <<
        //           xpos
        //           << std::setw(10) << "\typos: " << ypos << std::flush;

        // Sleep if render finishes early
        // double frame_start = curr_time;
        // double frame_end = glfwGetTime();
        // double frame_duration = frame_end - frame_start;

        // if (frame_duration < FRAME_TARGET) {
        //     double sleep_duration = FRAME_TARGET - frame_duration;
        //     std::this_thread::sleep_for(
        //         std::chrono::milliseconds(int(sleep_duration * 1000)));
        // }
    }

    window->terminate();
    return EXIT_SUCCESS;
}
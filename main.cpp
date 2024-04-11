#include <chrono>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <thread>

#include "component/component.h"
#include "engine/ecs/manager.h"
#include "engine/event/event.h"
#include "engine/renderer/shader.h"
#include "engine/window/window_factory.h"

const int SRC_HEIGHT = 600;
const int SRC_WIDTH = 800;

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
                 std::shared_ptr<transform_component> transform, color_component& color,
                 shader& shader) {
    shader.set_mat4("model", transform->transform);
    shader.set_vec4("u_color", color.color);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int main() {
    // context_window* window = create_window(SRC_WIDTH, SRC_HEIGHT,
    // "bitghetti");
    glfw_window* window = new glfw_window(SRC_WIDTH, SRC_HEIGHT, "bitghetti");

    // Main rendering loop
    // -------------------

    double prev_time = window->get_time(), fps_timer = prev_time;
    double lag = 0.0;
    int frames = 0;
    int pframes = 0;
    // int updates = 0;

    shader quad_shader("shaders/test.vert", "shaders/test.frag");
    shader pick_shader("shaders/mouse_pick.vert", "shaders/mouse_pick.frag");

    auto t_manager = std::make_shared<transform_manager>();
    event_dispatcher::instance()->add_listener(t_manager);

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

    // Quad vertices
    GLfloat quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };
    // clang-format on

    mesh_component mesh(vertices, indices);
    color_component color(glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
    auto transform = std::make_shared<transform_component>(glm::mat4(1.0f));

    t_manager->add_transform(transform);

    unsigned int VAO = init_mesh(mesh);

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);

    // mouse picking framebuffer
    unsigned int pick_fbo;
    glGenFramebuffers(1, &pick_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, pick_fbo);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, SRC_WIDTH, SRC_HEIGHT, 0,
                 GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    auto [x_prev, y_prev] = window->get_cursor_pos();

    // glm::vec3 look_from = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 look_at = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // glm::mat4 projection =
    //     glm::perspective(glm::radians(45.0f),
    //                      (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

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
        // ---------------------------------

        // create transformations
        // transform.transform = glm::rotate(model, (float)glfwGetTime(),
        //                                   glm::vec3(0.5f, 1.0f, 0.0f));

        pick_shader.use();
        pick_shader.set_mat4("view", view);
        pick_shader.set_mat4("projection", projection);
        pick_shader.set_uint("entity_id", 5);

        const unsigned int clear_value = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, pick_fbo);
        glClearBufferuiv(GL_COLOR, 0, &clear_value);
        glClearDepth(1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_mesh(VAO, mesh, transform, color, quad_shader);

        // Read pixels from framebuffer and print value that matches mouse
        // position
        auto [x_curr, y_curr] = window->get_cursor_pos();
        unsigned int pixel;
        glReadPixels(x_curr, SRC_HEIGHT - y_curr, 1, 1, GL_RED_INTEGER,
                     GL_UNSIGNED_INT, &pixel);

        if (pixel == 5) {
            color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            t_manager->select(0);
        } else {
            color = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
            t_manager->clear_selection();
        }

        std::cout << "xpos: " << x_curr << ", ypos: " << y_curr
                  << ", value: " << pixel << ", square: " << transform->to_string() << std::endl;

        quad_shader.use();
        quad_shader.set_mat4("view", view);
        quad_shader.set_mat4("projection", projection);

        // render box
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_mesh(VAO, mesh, transform, color, quad_shader);

        window->update();
        frames++;

        // Update FPS
        if (window->get_time() - fps_timer > 1.0) {
            fps_timer += 1.0;
            pframes = frames;
            frames = 0;
        }
    }
    glDeleteFramebuffers(1, &pick_fbo);

    window->terminate();
    return EXIT_SUCCESS;
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
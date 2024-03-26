#include "glfw_window.h"

void framebuffer_size_callback(GLFWwindow* wndw, int width, int height) {
    glViewport(0, 0, width, height);
}

glfw_window::glfw_window(int width, int height, const std::string& title) {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(EXIT_FAILURE);
    }

    // Set the viewport size and register the framebuffer size callback
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void glfw_window::terminate() { glfwTerminate(); }

void glfw_window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void glfw_window::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::tuple<double, double> glfw_window::get_cursor_pos() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {xpos, ypos};
}

bool glfw_window::is_open() const { return !glfwWindowShouldClose(window); }
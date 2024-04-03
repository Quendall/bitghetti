#include "opengl_context.h"

opengl_context::opengl_context(GLFWwindow* window_handle)
    : window_handle(window_handle) {}

void opengl_context::init() {
    glfwMakeContextCurrent(window_handle);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
}

void opengl_context::swap_buffers() {
    glfwSwapBuffers(window_handle);
}

#include "glfw_window.h"

#include "../event/event.h"

void framebuffer_size_callback(GLFWwindow* wndw, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_move_callback(GLFWwindow* wndw, double xpos, double ypos) {
    // Retrieve the glfw_window instance from the GLFWwindow
    auto window = static_cast<glfw_window*>(glfwGetWindowUserPointer(wndw));

    float depth;
    glReadPixels(xpos, window->get_height() - ypos, 1, 1, GL_DEPTH_COMPONENT,
                 GL_FLOAT, &depth);
    mouse_move_event event(xpos, ypos, depth);
    event_dispatcher::instance()->dispatch_event(event);
}

glfw_window::glfw_window(int width, int height, const std::string& title) {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->width = width;
    this->height = height;

    // Create a window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    g_context = new opengl_context(window);
    g_context->init();

    // Set the viewport size and register the framebuffer size callback
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);

    // Store a pointer to this glfw_window instance in the GLFWwindow
    glfwSetWindowUserPointer(window, this);
}

void glfw_window::terminate() {
    glfwTerminate();
}

void glfw_window::update() {
    g_context->swap_buffers();
    glfwPollEvents();
}

void glfw_window::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // Mouse input
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        event_dispatcher::instance()->dispatch_event(mouse_down_event(true));
        mouse_down = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        event_dispatcher::instance()->dispatch_event(mouse_down_event(false));
        mouse_down = false;
    }
}

double glfw_window::get_time() {
    return glfwGetTime();
}

std::tuple<double, double> glfw_window::get_cursor_pos() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {xpos, ypos};
}

bool glfw_window::is_open() const {
    return !glfwWindowShouldClose(window);
}

glfw_window::~glfw_window() {
    delete g_context;
    glfwDestroyWindow(window);
}
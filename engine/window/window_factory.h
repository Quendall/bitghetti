#include "glfw_window.h"

context_window* create_window(int width, int height, const std::string& title) {
    return new glfw_window(width, height, title);
}
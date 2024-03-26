#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "context_window.h"

class glfw_window : public context_window {
  private:
    GLFWwindow* window;

  public:
    glfw_window(int width, int height, const std::string& title);
    void terminate();
    void update();
    void process_input();
    std::tuple<double, double> get_cursor_pos();
    bool is_open() const;
};
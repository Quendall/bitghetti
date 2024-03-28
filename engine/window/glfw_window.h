#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../renderer/opengl_context.h"
#include "context_window.h"

class glfw_window : public context_window {
  private:
    GLFWwindow* window;
    opengl_context* g_context;

  public:
    glfw_window(int width, int height, const std::string& title);
    void terminate() override;
    void update() override;
    void process_input() override;
    double get_time() override;
    std::tuple<double, double> get_cursor_pos() override;
    bool is_open() const override;
};
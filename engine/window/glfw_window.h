#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../renderer/opengl_context.h"
#include "context_window.h"

enum state {
    none = 0,
    mouse_down,
};

class glfw_window : public context_window {
  private:
    GLFWwindow* window;
    opengl_context* g_context;
    unsigned int width;
    unsigned int height;

  public:
    glfw_window(int width, int height, const std::string& title);
    ~glfw_window();
    void terminate() override;
    void update() override;
    void process_input() override;
    double get_time() override;
    std::tuple<double, double> get_cursor_pos() override;
    bool is_open() const override;
    unsigned int get_width() const override { return width; }
    unsigned int get_height() const override { return height; }
    bool mouse_down = false;
};
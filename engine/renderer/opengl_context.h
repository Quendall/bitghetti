#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../core.h"
#include "graphics_context.h"

class opengl_context : graphics_context {
  public:
    opengl_context(GLFWwindow* window_handle);
    void init() override;
    void swap_buffers() override;

  private:
    GLFWwindow* window_handle;
};
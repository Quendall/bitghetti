#pragma once

#include <glm/glm.hpp>

#include "../../core.h"
using glm::mat4;

struct transform_component {
    mat4 transform;

    transform_component() = default;
    transform_component(const mat4& transform) : transform(transform) {}
    transform_component(const transform_component&) = default;
    transform_component(transform_component&&) = default;

    // operator=(mat4 other) { transform = other; }
    operator mat4&() {
        return transform;
    }
    operator const mat4&() const {
        return transform;
    }
    std::string to_string() const {
        vec3 pos = transform * glm::vec4(0, 0, 0, 1.0f);
        return "[" + std::to_string(pos.x) + ", " + std::to_string(pos.y) +
               ", " + std::to_string(pos.z) + "]";
    }
};

struct color_component {
    glm::vec4 color;

    color_component() = default;
    color_component(const glm::vec4& color) : color(color) {}
    color_component(const color_component&) = default;
    color_component(color_component&&) = default;
    color_component& operator=(const glm::vec4& new_color) noexcept {
        color = new_color;
        return *this;
    }
};

struct mesh_component {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    mesh_component() = default;
    mesh_component(const std::vector<float>& vertices,
                   const std::vector<unsigned int>& indices)
        : vertices(vertices), indices(indices) {}
    mesh_component(const mesh_component&) = default;
    mesh_component(mesh_component&&) = default;
};

struct camera_component {
    mat4 projection;
    mat4 view;

    camera_component() = default;
    camera_component(const mat4& projection, const mat4& view)
        : projection(projection), view(view) {}
    camera_component(const camera_component&) = default;
    camera_component(camera_component&&) = default;
};
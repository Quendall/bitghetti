#include <glm/glm.hpp>

#include "../../core.h"
using glm::mat4;

struct transform_component {
    mat4 transform;

    transform_component() = default;
    transform_component(const mat4& transform) : transform(transform) {}
    transform_component(const transform_component&) = default;
    transform_component(transform_component&&) = default;

    operator mat4&() { return transform; }
    operator const mat4&() const { return transform; }
};

struct color_component {
    glm::vec4 color;

    color_component() = default;
    color_component(const glm::vec4& color) : color(color) {}
    color_component(const color_component&) = default;
    color_component(color_component&&) = default;
};

struct mesh_component {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    mesh_component() = default;
    mesh_component(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
        : vertices(vertices), indices(indices) {}
    mesh_component(const mesh_component&) = default;
    mesh_component(mesh_component&&) = default;
};

struct camera_component {
    mat4 projection;
    mat4 view;

    camera_component() = default;
    camera_component(const mat4& projection, const mat4& view) : projection(projection), view(view) {}
    camera_component(const camera_component&) = default;
    camera_component(camera_component&&) = default;
};
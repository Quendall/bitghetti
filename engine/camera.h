#pragma once

#include <glm/glm.hpp>

const float SENSITIVITY = 0.1f;

class camera {
    private:
    glm::vec3 position;
    glm::vec3 look_at;
    glm::vec3 up;

    float mouse_sensitivity;
    float zoom;

    public:
    camera() = default;
    camera(glm::vec3 look_from, glm::look_at, glm::vec3 up)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)),
          mouse_sensitivity(SENSITIVITY),
          zoom(0.0f) {
        this->position = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }
}
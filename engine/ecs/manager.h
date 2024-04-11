#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "../event/event.h"
#include "component/component.h"

using sp_trans_comp = std::shared_ptr<transform_component>;

const int WIDTH = 800;
const int HEIGHT = 600;

class transform_manager : public event_listener {
  public:
    virtual ~transform_manager() = default;
    void on_event(const event& e) override {
        // Handle events
        if (const mouse_down_event* mde =
                dynamic_cast<const mouse_down_event*>(&e)) {
            mouse_down = mde->down;
        }

        if (const mouse_move_event* mme =
                dynamic_cast<const mouse_move_event*>(&e)) {

            double norm_x = (2 * mme->x / WIDTH) - 1;
            double norm_y = 1 - (2 * mme->y / HEIGHT);
            double norm_z = 2 * mme->z - 1;

            if (mouse_down) {
                for (auto id : selected) {
                    transforms[id]->transform = glm::translate(
                        glm::mat4(1.0f), glm::vec3(norm_x, norm_y, 0.0f));
                }
            }
        }
    }

    void add_transform(sp_trans_comp transform) {
        transforms.push_back(transform);
    }

    void select(size_t id) {
        selected.push_back(id);
    }

    void clear_selection() {
        selected.clear();
    }

    glm::vec4 unproject(double x, double y, double z) {
        double norm_x = (2 * x / WIDTH) - 1;
        double norm_y = 1 - (2 * y / HEIGHT);
        double norm_z = 2 * z - 1;

        glm::mat4 view =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 proj = (glm::perspective(
            glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));
        glm::mat4 inv_proj = glm::inverse(proj * view);

        glm::vec4 world_coords =
            inv_proj * glm::vec4(norm_x, norm_y, norm_z, 1.0f);

        return world_coords;
    }

  private:
    bool mouse_down = false;
    glm::vec2 mouse_pos;
    std::vector<sp_trans_comp> transforms;
    std::vector<size_t> selected;
};
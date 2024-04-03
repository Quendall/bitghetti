// #pragma once

// #include <typeindex>
// #include <unordered_map>
// #include <unordered_set>

// #include "../../core.h"
// #include "../component/component.h"

// class entity {
//   public:
//     unsigned int id;
//     std::unordered_set<std::type_index> component_types;
// };

// class entity_manager {
//   private:
//     std::unordered_map<unsigned int, entity> entities; // map of entity id to entity
//     std::unordered_map<std::type_index,
//                        std::unordered_map<unsigned int, component*>>
//         components;

//   public:
//     template <typename T, typename... Args>
//     T& add_component(unsigned int entity_id, Args&&... args) {
//         T* comp = new T(std::forward<Args>(args)...);
//         components[typeid(T)][entity_id] = comp;
//         entities[entity_id].component_types.insert(typeid(T));
//         return *comp;
//     }

//     template <typename T>
//     T& get_component(unsigned int entity_id) {
//         return *static_cast<T*>(components[typeid(T)][entity_id]);
//     }

//     template <typename T>
//     bool has_component(unsigned int entity_id) {
//         return entities[entity_id].component_types.count(typeid(T)) > 0;
//     }
// };

// class movement_system {
//   public:
//     void update(entity_manager& manager, float dt) {
//         for (auto& pair : manager.entities) {
//             if (manager.has_component<position_component>(pair.first) &&
//                 manager.has_component<velocity_component>(pair.first)) {
//                 position_component& pos =
//                     manager.get_component<position_component>(pair.first);
//                 velocity_component& vel =
//                     manager.get_component<velocity_component>(pair.first);
//                 pos.x += vel.speed * dt * cos(vel.direction);
//                 pos.y += vel.speed * dt * sin(vel.direction);
//             }
//         }
//     }
// };
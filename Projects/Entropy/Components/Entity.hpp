
#include "Entt/single_include/entt/entt.hpp"

#pragma once

namespace Entropy {

    class Entity {
      private:
        std::weak_ptr<entt::registry> registryReference;
        entt::entity                  entity;

      public:
        Entity(std::shared_ptr<entt::registry> _registry) : registryReference(_registry), entity(_registry->create()) {}

        template <typename Component>
        decltype(auto) get() {
            if (auto registry = registryReference.lock()) return registry->get<Component>(entity);
            throw std::runtime_error("Registry reference invalid");
        }

        template <typename Component, typename... Args>
        decltype(auto) emplace(Args &&... args) {
            if (auto registry = registryReference.lock())
                return registry->emplace<Component>(entity, std::forward<Args>(args)...);
            throw std::runtime_error("Registry reference invalid");
        }

        template <typename... Component>
        bool has() {
            if (auto registry = registryReference.lock()) return registry->has<Component &&...>(entity);
            throw std::runtime_error("Registry reference invalid");
        }

        ~Entity() {}
    };

}  // namespace Entropy

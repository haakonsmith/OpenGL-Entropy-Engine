/*
 * Copyright 2020, Haakon Smith.
 */

#include "../Graphics/Renderable.hpp"
#include "../Graphics/Shapes/Shape.hpp"
#include "../Physics/PhysicsObject.hpp"
#include "glm/glm.hpp"

#pragma once

using namespace glm;

namespace Entropy {

    class GameObject : public Renderable, public PhysicsObject {
      public:
        void setPosition(const vec3 &v) override {
            GameObject::PhysicsObject::data.position = GameObject::Renderable::transform.position = v;
        }
        vec3 getPosition() override { return GameObject::Renderable::transform.position; }

        GameObject();
        GameObject(Shape shape);
        ~GameObject();
    };

}  // namespace Entropy

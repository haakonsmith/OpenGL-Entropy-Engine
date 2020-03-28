#include "../Graphics/Renderable.hpp"
#include "../Graphics/Shapes/Shape.hpp"
#include "../Physics/PhysicsObject.hpp"
#include "glm/glm.hpp"

#pragma once

using namespace glm;

namespace Entropy
{

class GameObject : public Renderable, public PhysicsObject
{

public:
    void setPosition(const vec3 &v) override
    {
        GameObject::PhysicsObject::position = v;
        GameObject::Renderable::position = v;
    }
    vec3 getPosition() override { return GameObject::Renderable::position; }

    virtual void setModelMatrix(mat4 m) override
    {
        Renderable::modelMatrix = m;
        PhysicsObject::modelMatrix = m;
    }

    virtual mat4 getModelMatrix() override { return Renderable::modelMatrix; }

    GameObject();
    GameObject(Shape shape);
    ~GameObject();
};

} // namespace Entropy

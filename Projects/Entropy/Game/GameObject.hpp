#include "../Graphics/Renderable.hpp"
#include "../Physics/PhysicsObject.hpp"
#include "glm/glm.hpp"
#include "../Graphics/Shapes/Shape.hpp"

#pragma once

using namespace glm;

namespace Entropy
{

class GameObject : public Renderable, public PhysicsObject {

public:
    void setPosition(vec3 v) {GameObject::PhysicsObject::position = v; GameObject::Renderable::position = v;}
    vec3 getPosition() {return GameObject::Renderable::position;}

    virtual void setModelMatrix(mat4 m) {Renderable::modelMatrix = m; PhysicsObject::modelMatrix = m;}

    virtual mat4 getModelMatrix() {return Renderable::modelMatrix;}

    void setMVP(mat4 mvp) override {GameObject::PhysicsObject::setMVP(mvp); GameObject::Renderable::setMVP(mvp);}

    GameObject();
    GameObject(Shape shape);
    ~GameObject();
};

} // namespace Entropy

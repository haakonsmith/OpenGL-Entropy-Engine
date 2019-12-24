#include "../Graphics/Renderable.hpp"
#include "../Physics/PhysicsObject.hpp"
#include <glm/glm.hpp>

#pragma once

using namespace glm;

namespace Entropy
{

class GameObject : public Renderable, public PhysicsObject {

public:
    void setPosition(vec3 v) {GameObject::PhysicsObject::position = v; GameObject::Renderable::position = v;}
    vec3 getPosition() {return GameObject::Renderable::position;}

    GameObject();
    ~GameObject();
};

} // namespace Entropy

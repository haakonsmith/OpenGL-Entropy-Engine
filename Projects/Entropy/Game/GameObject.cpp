#include "GameObject.hpp"

namespace Entropy
{

GameObject::GameObject() : Renderable(), PhysicsObject() {
    PhysicsObject::name = "GameObject";
    Renderable::name = "GameObject";
}

GameObject::GameObject(Shape shape) : Renderable(shape), PhysicsObject(shape) {
    PhysicsObject::name = "GameObject";
    Renderable::name = "GameObject";
}

GameObject::~GameObject() {

}

} // namespace Entropy

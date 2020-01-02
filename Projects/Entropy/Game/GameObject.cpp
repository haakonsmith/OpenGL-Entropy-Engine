#include "GameObject.hpp"

namespace Entropy
{
    
GameObject::GameObject() : Renderable(), PhysicsObject() {

}

GameObject::GameObject(Shape shape) : Renderable(shape), PhysicsObject(shape) {

}

GameObject::~GameObject() {

}

} // namespace Entropy

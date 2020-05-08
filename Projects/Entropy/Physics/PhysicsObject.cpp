#include "PhysicsObject.hpp"

namespace Entropy {

    PhysicsObject::PhysicsObject() {
        data.position = vec3(0);
        data.velocity = vec3(0);
        data.physicsType = ACTIVE;
        name = "PhysicsObject";
    }

    PhysicsObject::~PhysicsObject() {}

}  // namespace Entropy

#include "PhysicsObject.hpp"

namespace Entropy
{
    
PhysicsObject::PhysicsObject() : position(0,0,0), velocity(0,0,0), collisionType(AABBCOLLISION) {
    vertices = {
        -1.0f, -1.0f, 0.0f, // x,y,z vertex 1
        1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
        1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
    };
    name = "PhysicsObject";
}

PhysicsObject::~PhysicsObject() {

}

} // namespace Entropy

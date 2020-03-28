#include "PhysicsObject.hpp"
#include <glm/glm.hpp>

#pragma once

using namespace glm;

struct collisionResult {
  bool collision = false;
}

collisionResult SATcollideSAT(PhysicsObject& obj1, PhysicsObject& obj2)
{ 
  vec2 axis(1,-1);
}
#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace Entropy
{

class PhysicsObject
{

public:
    virtual void setPosition(vec3 v) {position = v;}

    double friction = 1;
    vec3 position;
    vec3 velocity;

    PhysicsObject();
    ~PhysicsObject();
};

} // namespace Entropy

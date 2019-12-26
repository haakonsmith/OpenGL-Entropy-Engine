#pragma once

#include <glm/glm.hpp>

using namespace glm;

namespace Entropy
{

class PhysicsObject
{

public:
    virtual void setPosition(vec3 v) {position = v;}
    virtual vec3 getPosition() {return position;}

    double friction = 1;
    vec3 position;
    vec3 velocity;

    PhysicsObject();
    ~PhysicsObject();
};

} // namespace Entropy

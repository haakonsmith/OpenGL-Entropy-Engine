
#include "Components.hpp"

#pragma once

namespace Entropy {

    struct PhysicsData {
        vec3 velocity;
        vec3 acceleration;
        float friction;

        PhysicsData() : velocity(0), acceleration(0), friction(0) {}
        PhysicsData(vec3 _velocity, vec3 _acceleration, float _friction) : velocity(_velocity), acceleration(_acceleration), friction(_friction) {}
    };

    struct PhysicsSimData : public Translation {
        enum { STATIC, ACTIVE };

        double friction = 1;

        int physicsType = STATIC;

        vec3 velocity;
    };

}  // namespace Entropy


#include "Location.hpp"

#pragma once


namespace Entropy {

    struct PhysicsData : public Location {
        enum { STATIC, ACTIVE };

        double friction = 1;

        int physicsType = STATIC;

        vec3 velocity;
    };

}  // namespace Entropy

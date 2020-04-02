
#include "Cute/cute_c2.hpp"

#pragma once

namespace Entropy {

    enum CollisionDirection { TOP, BOTTOM, LEFT, RIGHT };

    struct CollisionData {
        bool collision;
        c2Manifold manifold;
    };

}  // namespace Entropy

#pragma once

namespace Entropy
{
    

enum CollisionDirection {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

struct CollisionData
{
    bool collision;
    CollisionDirection collisionDirection;
};

} // namespace Entropy
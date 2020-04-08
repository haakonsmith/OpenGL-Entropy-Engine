
#include "../Graphics/Shapes/Shape.hpp"
#include <glm/vec2.hpp>

#include "Cute/cute_c2.hpp"

using glm::vec2;

#pragma once

namespace Entropy {
    struct Collider {
        bool collidedLastFrame = false;
    };

    struct BoundingBoxCollider : public Collider {
        
        BoundingBox boundingBox;

        c2AABB getAABB(vec2 const& position) {
            c2AABB aabb;
            aabb.min = c2V((-boundingBox.width + position.x), (-boundingBox.height + position.y));
            aabb.max = c2V((boundingBox.width + position.x), (boundingBox.height + position.y));
            return aabb;
        }
    };

}  // namespace Entropy

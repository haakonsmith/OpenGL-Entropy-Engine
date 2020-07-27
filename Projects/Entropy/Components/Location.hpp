
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

using glm::vec2;
using glm::vec3;

#pragma once

namespace Entropy {
    struct Location {
        union {
            vec2 position2D;
            vec3 position;
        };

        Location(const vec3 &p) : position(p) {}
        Location() : position(vec3(0)) {}

        inline void setPosition(const vec3 &v) { position = v; }
        inline void setPosition(const vec2 &v) { setPosition(vec3(v, position.z)); }
        inline void setPosition(float x, float y) { setPosition(vec3(x, y, position.z)); }
        inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
        inline vec3 getPosition() { return position; }
    };

}  // namespace Entropy

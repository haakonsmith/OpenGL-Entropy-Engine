
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

using glm::vec3;
using glm::vec2;

#pragma once


namespace Entropy {
    struct Location {
        vec3 position;

        Location(vec3 const& p) : position(p) {}
        Location() : position(vec3(0)) {}

        inline void setPosition(const vec3 &v) { position = v; }
        inline void setPosition(const vec2 &v) { setPosition(vec3(v, position.z)); }
        inline void setPosition(float x, float y) { setPosition(vec3(x, y, position.z)); }
        inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
        inline vec3 getPosition() { return position; }
    };

}  // namespace Entropy

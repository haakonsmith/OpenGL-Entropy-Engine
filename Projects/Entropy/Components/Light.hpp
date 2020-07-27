
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#pragma once

using glm::vec3;
using glm::vec4;

namespace Entropy {

    struct Light {
        vec3 colour;
        vec3 position;
        float intensity;

        Light() : position(vec3(0)), intensity(10), colour(vec3(1)) {}
        Light(vec3 const &v) : position(v), intensity(10), colour(vec3(1)) {}
    };
}  // namespace Entropy

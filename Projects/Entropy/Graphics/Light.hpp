
#include "glm/vec3.hpp"

#pragma once

using glm::vec3;

namespace Entropy {

    struct Light {
        vec3 colour;
        vec3 position;
        float intensity;

        Light() : position(vec3(0)), intensity(0), colour(vec4(1)) {}
        Light(vec3 const &v) : position(v), intensity(0), colour(vec4(1)) {}
    };
}  // namespace Entropy

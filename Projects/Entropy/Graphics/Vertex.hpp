#include "glm/glm.hpp"

#pragma once

struct Vertex
{
    union {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float X, Y, Z;
        };
        glm::vec3 Position;
    };
    union {
        struct
        {
            float u, v;
        };
        struct
        {
            float U, V;
        };
        glm::vec2 UV;
    };

    Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}
    Vertex(float x, float y, float z) : x(x), y(y), z(z), u(0), v(0) {}
    Vertex(const glm::vec3 &position, const glm::vec2 &uv) : Position(position), UV(uv) {}
    Vertex(const glm::vec3 &position) : Position(position), UV(glm::vec2(1, 1)) {}
};

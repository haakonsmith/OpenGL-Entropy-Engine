/*
 * Copyright 2020, Haakon Smith.
 */

#include <vector>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include "../Vertex.hpp"
#include "Cute/cute_c2.hpp"
#include "glm/glm.hpp"

#pragma once

using namespace glm;
using namespace std;

namespace Entropy {

    struct Shape {
        vector<Vertex> Vertices;
        Shape() {}
    };

    struct Rectangle : Shape {
        Rectangle() {
            Vertices = {
                // tri 1
                Vertex(-1.0f, -1.0f, 0.0f, 0, 0),
                Vertex(1.0f, -1.0f, 0.0f, 1, 0),
                Vertex(1.0f, 1.0f, 0.0f, 1, 1),

                // tri 2
                Vertex(-1.0f, -1.0f, 0.0f, 0, 0),
                Vertex(-1.0f, 1.0f, 0.0f, 0, 1),
                Vertex(1.0f, 1.0f, 0.0f, 1, 1),
            };
        }
    };

    struct Line : Shape {
        Line(vec3 p1, vec3 p2) {
            Vertices.reserve(2);
            Vertices.push_back(p1);
            Vertices.push_back(p2);
        }
    };

}  // namespace Entropy

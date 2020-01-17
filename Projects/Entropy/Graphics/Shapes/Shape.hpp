
#include <vector>
#define GL_SILENCE_DEPRECATION
#include "glm/glm.hpp"
#include <OpenGL/gl3.h>

#pragma once

using namespace glm;
using namespace std;

namespace Entropy
{



struct Shape
{
    vector<GLfloat> vertices;

    vector<GLfloat> UVs;

    Shape() {
      vertices = {
        -1.0f, -1.0f, 0.0f, // x,y,z vertex 1
        1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
        1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
      };
      UVs = {
        0, 0,
        1, 0,
        1, 1
      };
    }

};

struct Rectangle : Shape
{

    Rectangle() {
      vertices = {
          // tri 1
          -1.0f, -1.0f, 0.0f, // x,y,z vertex 1
          1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
          1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
          // tri 2
          -1.0f, -1.0f, 0.0f, // x,y,z vertex 4
          -1.0f, 1.0f, 0.0f,  // x,y,z vertex 5
          1.0f, 1.0f, 0.0f,   // x,y,z vertex 6
      };

      UVs = {
      // tri 1
        0, 0, // x,y vertex 1
        1, 0, // x,y vertex 2
        1, 1, // x,y vertex 3
      //   tri 2
        0, 0, // x,y vertex 1
        0, 1, // x,y vertex 2
        1, 1, // x,y vertex 3
      };
    }
};

struct Line : Shape
{

    Line(vec3 p1, vec3 p2) {
      vertices.reserve(6);
      vertices.push_back(p1.x);
      vertices.push_back(p1.y);
      vertices.push_back(p1.z);
      vertices.push_back(p1.x);
      vertices.push_back(p1.y);
      vertices.push_back(p1.z);

    }
};

struct BoundingBox {
  float width, height;

  BoundingBox() : width(), height() {}
};

} // namespace Entropy


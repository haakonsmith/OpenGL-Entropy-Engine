#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#pragma once

class Renderable
{

public:
  vec3 Position;
  mat4 MVP;
  GLuint bufferobject;
  vector<GLfloat> vertices;
  


  // create default Renderable
  Renderable(vector<GLfloat> _vertices) {
    vertices = _vertices;
    Position = vec3(0,0,0);
  }

  // change Renderable Position
  Renderable(vector<GLfloat> _vertices, vec3 _position) {
    vertices = _vertices;
    Position = _position;
  }
  
  ~Renderable() {
    glDeleteBuffers(1, &bufferobject);
  }
};



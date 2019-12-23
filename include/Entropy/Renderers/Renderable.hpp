#include <OpenGL/gl3.h>

#pragma once

class Renderable
{
private:
  /* data */
public:
  GLuint bufferobject;
  
  ~Renderable() {
    glDeleteBuffers(1, &bufferobject);
  }
};



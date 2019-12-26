#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#pragma once

class Renderable
{

public:
  vec3 position;
  vec3 scale;
  double rotation;
  mat4 MVP;

  GLuint vertexBufferID;
  GLuint UVBufferID;
  GLuint TextureID;
  GLuint texture;
  
  vector<GLfloat> vertices;
  vector<GLfloat> UVs;
  virtual void setPosition(vec3 v) {position = v;}
  virtual vec3 getPosition() {return position;}



  // create default Renderable
  Renderable() : position(0,0,0), scale(10,10,10), rotation(0) {
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

  Renderable(vector<GLfloat> _vertices) : position(0,0,0), scale(10,10,10), rotation(0) {
    vertices = _vertices;
  }

  // change Renderable Position
  Renderable(vector<GLfloat> _vertices, vec3 _position) : scale(10,10,10), rotation(0) {
    vertices = _vertices;
    position = _position;
  }
  
  ~Renderable() {
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &UVBufferID);
    glDeleteTextures(1, &texture);
  }
};



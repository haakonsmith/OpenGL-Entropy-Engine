#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include "glm/glm.hpp"
#include "Shapes/Shape.hpp"
#include <vector>


using namespace glm;
using namespace std;

#pragma once
namespace Entropy
{
  

class Renderable
{

public:
  vec3 position;
  vec3 scale;
  double rotation;
  mat4 MVP;
  mat4 modelMatrix, viewMatrix, projectionMatrix;
  mat4 scaleMatrix, translationMatrix, rotationMatrix;

  GLuint vertexBufferID;
  GLuint UVBufferID;
  GLuint TextureID;
  GLuint texture;

  bool TextureINIT = false, UVBufferINIT = false, vertexBufferINIT = false;

  bool isLight = false;
  
  vector<GLfloat> vertices;
  vector<GLfloat> UVs;
  virtual void setPosition(vec3 v) {position = v;}
  virtual vec3 getPosition() {return position;}
  virtual void setMVP(mat4 mvp) {MVP = mvp;}
  virtual mat4 getMVP() {return MVP;}
  virtual void setModelMatrix(mat4 m) {modelMatrix = m;}

  virtual mat4 getModelMatrix() {return modelMatrix;}




  void setTexture(GLuint tex) {texture = tex; TextureINIT = true;}


  // create default Renderable
  Renderable() : position(0,0,0), scale(1,1,1), rotation(0) {
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

  Renderable(Shape shape) : position(0,0,0), scale(1,1,1), rotation(0){
    vertices = shape.vertices;
    UVs = shape.UVs;
  }

  Renderable(vector<GLfloat> _vertices) : position(0,0,0), scale(1,1,1), rotation(0) {
    vertices = _vertices;
    UVs = {
      0, 0,
      1, 0,
      1, 1
    };
  }

  // change Renderable Position
  Renderable(vector<GLfloat> _vertices, vec3 _position) : scale(1,1,1), rotation(0) {
    vertices = _vertices;
    position = _position;
  }
  
  ~Renderable() {
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &UVBufferID);
    glDeleteTextures(1, &texture);
  }
};

} // namespace Entropy


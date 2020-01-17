#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include "glm/glm.hpp"
#include "Shapes/Shape.hpp"
#include <string>
#include <vector>


using namespace glm;
using namespace std;

#pragma once
namespace Entropy
{

class Renderable
{

protected:

public:
  string name; 
  vec3 position;
  vec3 scale;
  double rotation;
  mat4 MVP;
  mat4 modelMatrix;
  mat4 scaleMatrix, translationMatrix, rotationMatrix;

  GLuint vertexBufferID;
  GLuint UVBufferID;
  GLuint TextureID;
  GLuint texture;

  bool TextureINIT = false, UVBufferINIT = false, vertexBufferINIT = false;

  bool isLight = false;
  
  vector<GLfloat> vertices;
  vector<GLfloat> UVs;

  short id;

  // Getters and Setters
  virtual void setPosition(vec3 v) {position = v;}
  inline virtual void setPosition(vec2 v) {setPosition(vec3(v, position.z));}
  inline virtual void setPosition(float x, float y) {setPosition(vec3(x, y, position.z));}
  inline virtual void setPosition(float x, float y, float z) {setPosition(vec3(x, y, z));}
  virtual vec3 getPosition() {return position;}

  virtual  void setScale(vec3 v) {scale = v;}
  inline virtual void setScale(float x, float y) {setScale(vec3(x, y, scale.z));}
  inline virtual void setScale(float x, float y, float z) {setScale(vec3(x, y, z));}
  inline virtual void setScale(vec2 v) {setScale(vec3(v, scale.z));}
  virtual vec3 getScale() {return scale;}

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
    name = "Renderable";
  }

  Renderable(Shape shape) : position(0,0,0), scale(1,1,1), rotation(0){
    vertices = shape.vertices;
    UVs = shape.UVs;
    name = "Renderable";
  }

  Renderable(vector<GLfloat> _vertices) : position(0,0,0), scale(1,1,1), rotation(0) {
    vertices = _vertices;
    UVs = {
      0, 0,
      1, 0,
      1, 1
    };
    name = "Renderable";
  }

  // change Renderable Position
  Renderable(vector<GLfloat> _vertices, vec3 _position) : scale(1,1,1), rotation(0) {
    vertices = _vertices;
    position = _position;
    name = "Renderable";
  }
  
  ~Renderable() {
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &UVBufferID);
    glDeleteTextures(1, &texture);
  }
};

} // namespace Entropy


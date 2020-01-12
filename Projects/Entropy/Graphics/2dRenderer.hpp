#ifndef M_2DRENDERER
  #define GL_SILENCE_DEPRECATION
  #define M_2DRENDERER
  #include <GLFW/glfw3.h>
  #include <OpenGL/gl3.h>
  #include <fstream>
  #include <iostream>
  #include <sstream>
  #include <vector>
  #include "Renderable.hpp"
  #include "shader.hpp"
  // #define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
  // #include <glm/gtx/string_cast.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

  
  
  
#else
#error "2d renderer included twice"
#endif

#pragma once

#define NDEBUG
#define SUPRESS

#ifndef COORDINATES
#define COORDINATES

enum Coordinates 
{
  x,
  y,
  z
};

#endif

#ifdef NDEBUG
#define GL_LOG(LOCATION) if(auto error = glGetError()) std::cout << "OpenGL error " << error << " at " << LOCATION << " " << __LINE__ << std::endl
#else
#define GL_LOG() do { } while(0)
#endif

#ifdef NDEBUG
#define LOG(MESSAGE) std::cout << MESSAGE << " at line:" << __LINE__ << std::endl
#else
#define LOG() do { } while(0)
#endif

namespace Entropy
{
  class m_2dRenderer
  {
    private:
      unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

      bool debugOutline = false, debugCenter = false;

  
      mat4 viewMatrix, projectionMatrix;

      GLuint VertexArrayID;
      GLuint programID;
      GLuint debugCenterShader;
      GLuint debugShaderID;
      GLuint debugLineShaderID;
      std::vector<Renderable*> objects = std::vector<Renderable*>();



      
  
      /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
      void renderCenter(Renderable* _renderable);
  
      /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
      void renderOutline(Renderable* _renderable);
  

      void genVertexBuffer(Renderable* _renderable);
      void genUVBuffer(Renderable* _renderable);


    public:
      /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
      glm::vec3 modelSpace(glm::vec3 coords);

      glm::vec3 worldSpace(glm::vec3 coords);
      /**
        * Draw line between two world space points
        */
      void renderLine(glm::vec3 p1, glm::vec3 p2);

      GLuint loadTexture(std::string path);
      /**
        * Add renderable object to objects.
        * 
        * Required for some renderer optimizations
        */
      void addRenderable(Renderable* _renderable);

      /**
       * Render a renderable without adding to the renderer.
       */ 
      void render(Renderable* obj);

      /**
       * Generate all buffers for a renderables.
       */ 
      void buffer(Renderable* _renderable);

      /**
       * Render all renderables in renderer.
       */ 
      void renderFrame();


      /**
       * Enables drawing of the center of graphics, for debugging.
       * !!!!BROKEN!!!!
       */
      void drawCenter(bool val) {debugCenter = val;};

      /**
       * Enables drawing of the outline of graphics, for debugging.
       * Different to collision outline debug.
       * !!!!BROKEN!!!!
       */
      void drawOutline(bool val) {debugOutline = val;};

      /**
       * Update renderable model view matrix
       */
      void transform(Renderable* obj);

      m_2dRenderer(unsigned int width, unsigned int height);
      ~m_2dRenderer();
    };
} // namespace Entropy

#ifndef M_2DRENDERER
  #define M_2DRENDERER
  #include <GLFW/glfw3.h>
  #include <OpenGL/gl3.h>
  #include <fstream>
  #include <iostream>
  #include <sstream>
  #include <vector>
  #include "Renderable.hpp"
  #include <main/shader.hpp>
  #include <glm/glm.hpp>
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/type_ptr.hpp>

  
#else
#error "2d renderer included twice"
#endif

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

      GLuint VertexArrayID;
      GLuint programID;
      std::vector<Renderable*> objects = std::vector<Renderable*>();



      /**
       * Scaling factor for render space coordinates
       */
      int coordinateSpace = 20;

      /**
       * Converts coordinate space coordinate to OpenGL float space
       */
      float coordinate_transform(float coord, int dire);

      /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
      glm::vec3 coordinate_transform(glm::vec3 coords);
  

    public:
      /**
        * Add renderable object to objects.
        * 
        * Required for some renderer optimizations
        */
      void add_renderable(Renderable* _renderable);
      void render(Renderable* obj);
      void renderFrame();
      /**
       * Update renderable model view matrix
       */
      void transform(Renderable* obj);

      m_2dRenderer(unsigned int width, unsigned int height);
      ~m_2dRenderer();
    };
} // namespace Entropy
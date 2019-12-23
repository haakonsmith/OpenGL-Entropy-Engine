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


#ifdef NDEBUG
#define GL_LOG(LOCATION) if(auto error = glGetError()) std::cout << "OpenGL error " << error << " at " << LOCATION << " " << __LINE__ << std::endl
#else
#define GL_LOG() do { } while(0)
#endif

namespace Entropy
{
  class m_2dRenderer
  {
    private:
      GLuint VertexArrayID;
      GLuint programID;
      std::vector<Renderable*> objects = std::vector<Renderable*>();

      void render(Renderable* obj);

      

    public:
    
      void add_renderable(Renderable* _renderable);
      void renderFrame();
      m_2dRenderer(/* args */);
      ~m_2dRenderer();
    };
} // namespace Entropy
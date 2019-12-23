
#ifdef NDEBUG
#define GL_LOG() if (glGetError() == 0) std::cout << "OpenGL error " << glGetError() << std::endl;
#else
#define GL_LOG() do { } while(0)
#endif

#include "Application.hpp"

#include "Renderers/Renderable.hpp"

#include "Renderers/2dRenderer.hpp"
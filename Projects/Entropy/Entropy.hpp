
#ifdef NDEBUG
#define LOG(MESSAGE) std::cout << MESSAGE << " at line:" << __LINE__ << std::endl
#else
#define LOG() do { } while(0)
#endif

#include "Application.hpp"

#include "Graphics/Renderable.hpp"

#include "Graphics/2dRenderer.hpp"

#include "Graphics/Shapes/Shape.hpp"

#include "Physics/PhysicsEngine.hpp"
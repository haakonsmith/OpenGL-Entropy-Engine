
#include "PhysicsEngine.hpp"

namespace Entropy
{

void PhysicsEngine::timeStep(float timeStep)
{
  // renderer.renderLine(vec3(0),
  //           vec3(10,100,1)
  //         );

    GL_LOG("renderLine");

    // LOG(renderer);
    for (auto obj : objects) {
      

        

        obj->velocity = obj->velocity -  ((obj->velocity * (float) (obj->friction * 10)) * timeStep);

        obj->setPosition(obj->position + (obj->velocity) * timeStep);
    }
}

PhysicsEngine::PhysicsEngine(m_2dRenderer & _renderer) : renderer(_renderer)
{
}

PhysicsEngine::~PhysicsEngine()
{
  // for (auto obj : objects)
  //   delete obj;

  objects.clear();

  std::cout << "Cleaning up physics engine" << std::endl;
}

} // namespace Entropy

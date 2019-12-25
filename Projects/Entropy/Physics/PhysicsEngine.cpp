
#include "PhysicsEngine.hpp"

namespace Entropy
{

void PhysicsEngine::timeStep(float timeStep)
{
    for (auto obj : objects) {
        obj->velocity = obj->velocity -  ((obj->velocity * (float) (obj->friction * 10)) * timeStep);

        obj->setPosition(obj->position + (obj->velocity) * timeStep);
    }
}

PhysicsEngine::PhysicsEngine(/* args */)
{
}

PhysicsEngine::~PhysicsEngine()
{
  for (auto obj : objects)
    delete obj;

  objects.clear();

  std::cout << "Cleaning up physics engine" << std::endl;
}

} // namespace Entropy

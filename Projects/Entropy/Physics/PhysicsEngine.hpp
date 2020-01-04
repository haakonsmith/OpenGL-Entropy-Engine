#include "glm/glm.hpp"
#include <vector>
#include "PhysicsObject.hpp"
#include <iostream>
#include "../Graphics/2dRenderer.hpp"

#pragma once


namespace Entropy
{
    
class PhysicsEngine
{
private:
    /* data */
public:
    m_2dRenderer& renderer;

    /**
     * list of objects
     */
    std::vector<PhysicsObject*> objects = std::vector<PhysicsObject*>();

    /**
     * Takes time step seconds.
     */
    void timeStep(float deltaTime);

    /**
     * Adds object to engine
     */
    void addObject(PhysicsObject* obj) {objects.push_back(obj);}

    PhysicsEngine(m_2dRenderer & _renderer);
    ~PhysicsEngine();
};

} // namespace Entropy

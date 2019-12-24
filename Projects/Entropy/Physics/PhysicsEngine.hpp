#include <glm/glm.hpp>
#include <vector>
#include "PhysicsObject.hpp"
#include <iostream>

#pragma once


namespace Entropy
{
    
class PhysicsEngine
{
private:
    /* data */
public:
    /**
     * list of objects
     */
    std::vector<PhysicsObject*> objects;

    /**
     * Takes time step seconds.
     */
    void timeStep(float deltaTime);

    /**
     * Adds object to engine
     */
    void addObject(PhysicsObject* obj) {objects.push_back(obj);}

    PhysicsEngine(/* args */);
    ~PhysicsEngine();
};

} // namespace Entropy

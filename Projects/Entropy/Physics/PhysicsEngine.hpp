#include "CollisionData.hpp"
#include "PhysicsObject.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

#include "../Graphics/2dRenderer.hpp"

#pragma once

namespace Entropy
{

class PhysicsEngine
{
private:
    /* data */
public:
    m_2dRenderer &renderer;

    float distToNearestPoint(vec3 point);
    float distToNearestPoint(vec3 point, PhysicsObject *obj);
    float distToNearestPoint(PhysicsObject *obj);

    float distToObject(vec3 point, PhysicsObject *_obj);
    float distToNearestObject(PhysicsObject *_obj);

    bool debug = false;

    static bool AABBIntersectionTest(PhysicsObject *obj1, PhysicsObject *obj2);
    static CollisionData AABBCollisionTest(PhysicsObject *obj1, PhysicsObject *obj2);
    /**
     * list of objects
     */
    std::vector<PhysicsObject *> objects = std::vector<PhysicsObject *>();

    /**
     * Takes time step seconds.
     */
    void timeStep(float deltaTime);

    /**
     * Adds object to engine
     */
    inline void addObject(PhysicsObject *obj) { objects.push_back(obj); }
    inline void removeObject(PhysicsObject *obj)
    {
        objects.erase(find(objects.begin(), objects.end(), obj));
    }

    PhysicsEngine(m_2dRenderer &_renderer);
    ~PhysicsEngine();
};

} // namespace Entropy

/*
 * Copyright 2020, Haakon Smith.
 */

#include <iosfwd>
#include <vector>

#include "../Graphics/2dRenderer.hpp"
#include "CollisionData.hpp"
#include "Cute/cute_c2.hpp"
#include "Entt/single_include/entt/entt.hpp"
#include "PhysicsObject.hpp"
#include "glm/glm.hpp"
#include "../Components/Components.hpp"


#pragma once

namespace Entropy {

    class PhysicsEngine {
        entt::registry &registry;

      public:
        m_2dRenderer &renderer;

        Screen &screen;

        float distToNearestPoint(vec3 point);
        float distToNearestPoint(vec3 point, PhysicsObject *obj);
        float distToNearestPoint(PhysicsObject *obj);

        float distToObject(vec3 point, PhysicsObject *_obj);
        float distToNearestObject(PhysicsObject *_obj);

        void checkForCollisions(PhysicsObject &obj, vec3 &prePos);

        void checkForCollision(PhysicsObject &obj, PhysicsObject &_obj, vec3 const &objPrePos);

        bool debug = false;

        void preventIntersection(entt::entity entityA, entt::entity entityB);

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
        inline void removeObject(PhysicsObject *obj) { objects.erase(find(objects.begin(), objects.end(), obj)); }

        inline c2v scaleForCollision(c2v const &p) {
            PROFILE_FUNCTION();
            return c2V(p.x / screen.sizeX + screen.sizeX, p.y / screen.sizeY + screen.sizeY);
        }

        inline c2AABB scaleForCollision(c2AABB const &aabb) {
            PROFILE_SCOPE("AABB scaleForCollision");
            c2AABB sAABB;
            sAABB.min = scaleForCollision(aabb.min);
            sAABB.max = scaleForCollision(aabb.max);
            return sAABB;
        }

        inline c2v scaleForRenderering(c2v const &p) {
            PROFILE_FUNCTION();
            return c2V((p.x - screen.sizeX) * screen.sizeX, (p.y - screen.sizeY) * screen.sizeY);
        }

        PhysicsEngine(m_2dRenderer &_renderer, Screen &_screen, entt::registry &_registry);
        ~PhysicsEngine();
    };

}  // namespace Entropy

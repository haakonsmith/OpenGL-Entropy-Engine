/*
 * Copyright 2020, Haakon Smith.
 */

#pragma once

#include <string>
#include <vector>

#include "../Graphics/Shapes/Shape.hpp"
#include "CollisionData.hpp"
#include "Cute/cute_c2.hpp"
#include "glm/glm.hpp"

#include "../Components/PhysicsData.hpp"
#include "../Components/Collider.hpp"

using namespace glm;

enum { BOUNDINGBOXCOLLISION, AABBCOLLISION, POLYGONCOLLISION, ACTIVE, STATIC };
namespace Entropy {

    class PhysicsObject {
        friend class PhysicsEngine;

      public:
        PhysicsData data;

        BoundingBoxCollider collider;

        inline c2AABB getAABB() { return collider.getAABB(data.position); }

        string name;

        virtual void setPosition(const vec3 &v) { data.position = v; }
        virtual vec3 getPosition() { return data.position; }

        virtual void update(){};

        virtual void customPrePhysicsStep(double deltaTime){};
        virtual void collide(vec3 prePos, PhysicsObject *collided, CollisionData data) {};


        inline bool is(PhysicsObject const &obj) { return (typeid(this) == typeid(obj)); }

        template <typename T>
        inline bool is() {
            return (typeid(*this) == typeid(T));
        }

        inline void preventIntersection(CollisionData data) {
            if (sign(this->data.velocity.x) == data.manifold.n.x) this->data.velocity.x = 0;
            if (sign(this->data.velocity.y) == data.manifold.n.y) this->data.velocity.y = 0;
        }

        PhysicsObject();
        ~PhysicsObject();
    };

}  // namespace Entropy

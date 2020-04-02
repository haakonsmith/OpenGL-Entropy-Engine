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

using namespace glm;

enum { BOUNDINGBOXCOLLISION, AABBCOLLISION, POLYGONCOLLISION, ACTIVE, STATIC };
namespace Entropy {

    class PhysicsObject {
        friend class PhysicsEngine;

      protected:
        mat4 modelMatrix;

      public:
        string name;
        int collisionType;
        int physicsType = STATIC;
        BoundingBox boundingBox;

        vector<Vertex> Vertices;

        bool collidedLastFrame = false;

        virtual void setPosition(const vec3 &v) { position = v; }
        virtual vec3 getPosition() { return position; }

        virtual void setModelMatrix(mat4 m) { modelMatrix = m; }
        virtual mat4 getModelMatrix() { return modelMatrix; }

        double friction = 1;
        vec3 position;
        vec3 velocity;

        virtual void update(){};

        virtual void customPrePhysicsStep(double deltaTime){};
        virtual void collide(vec3 prePos, PhysicsObject *collided, CollisionData data){};

        c2AABB getAABB() {
            c2AABB aabb;
            aabb.min = c2V((-boundingBox.width + getPosition().x), (-boundingBox.height + getPosition().y));
            aabb.max = c2V((boundingBox.width + getPosition().x), (boundingBox.height + getPosition().y));
            return aabb;
        }

        inline bool is(PhysicsObject const &obj) { return (typeid(this) == typeid(obj)); }

        template<typename T>
        inline bool is() { return (typeid(*this) == typeid(T)); }

        inline void preventIntersection(CollisionData data) {
          if (sign(velocity.x) == data.manifold.n.x)
              velocity.x = 0;
          if (sign(velocity.y) == data.manifold.n.y)
              velocity.y = 0;
        }

        PhysicsObject();
        PhysicsObject(Shape shape) : PhysicsObject() { Vertices = shape.Vertices; };
        ~PhysicsObject();
    };

}  // namespace Entropy

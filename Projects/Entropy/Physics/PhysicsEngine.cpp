#include "PhysicsEngine.hpp"

#include <future>
namespace Entropy {

    void PhysicsEngine::checkForCollisions(PhysicsObject &obj, vec3 &prePos) {
        PROFILE_FUNCTION();
        for (auto _obj : objects) {
            if (&obj != _obj) { checkForCollision(obj, *_obj, prePos); }
        }
    }

    void PhysicsEngine::checkForCollision(PhysicsObject &obj, PhysicsObject &_obj, vec3 const &objPrePos) {
        PROFILE_FUNCTION();
        CollisionData collision;

        collision.collision = c2AABBtoAABB(scaleForCollision(obj.getAABB()), scaleForCollision(_obj.getAABB()));

        if (collision.collision) {
            c2AABBtoAABBManifold(scaleForCollision(obj.getAABB()), scaleForCollision(_obj.getAABB()),
                                 &collision.manifold);
            obj.collider.collidedLastFrame = true;
            {
                PROFILE_SCOPE("CollisionHandling");
                obj.collide(objPrePos, &_obj, collision);
                _obj.collide(objPrePos, &obj, collision);
            }
        } else {
            obj.collider.collidedLastFrame = false;
        }
    }

    void PhysicsEngine::timeStep(float timeStep) {
        PROFILE_FUNCTION();
        // const int collisionPrecision = 1000;

        // future<void> futures[objects.size()];

        // for (size_t i = 0; i < objects.size(); i++) {
        //     auto obj = objects[i];
        //     if (obj->data.velocity != vec3(0) && obj->data.physicsType == ACTIVE) {
        //         vec3 prePos = obj->getPosition();

        //         obj->customPrePhysicsStep((float)timeStep);

        //         obj->data.velocity.x = floorf(obj->data.velocity.x * collisionPrecision) / collisionPrecision;
        //         obj->data.velocity.y = floorf(obj->data.velocity.y * collisionPrecision) / collisionPrecision;
        //         obj->data.velocity.z = floorf(obj->data.velocity.z * collisionPrecision) / collisionPrecision;
        //         obj->data.velocity =
        //             obj->data.velocity - ((obj->data.velocity * (float)(obj->data.friction * 10)) * timeStep);

        //         // futures[i] = std::async(std::launch::async, &PhysicsEngine::checkForCollisions, this,
        //         std::ref(*obj), std::ref(prePos)); checkForCollisions( std::ref(*obj), std::ref(prePos));

        //         obj->setPosition(obj->data.position + (obj->data.velocity) * timeStep);

        //         if (debug) {
        //             renderer.renderQuad(obj->getPosition(), obj->collider.boundingBox.width,
        //                                 obj->collider.boundingBox.height, true,
        //                                 obj->collider.collidedLastFrame ? vec3(0, 1, 0) : vec3(1, 0, 0));
        //         }
        //     }
        // }

        registry.group<Transform, PhysicsData>().each([timeStep](auto entity, auto &pos, auto &data) {
            data.velocity += data.acceleration * timeStep;

            data.velocity = data.velocity - data.velocity * (data.friction * timeStep);

            // if (distance(data.velocity, vec3(0)) < 1)
            //     data.velocity = vec3(0);

            pos.position = pos.position + data.velocity * timeStep;
        });
    }

    PhysicsEngine::PhysicsEngine(m_2dRenderer &_renderer, Screen &_screen, entt::registry &_registry)
        : registry(_registry), screen(_screen), renderer(_renderer) {}

    PhysicsEngine::~PhysicsEngine() {
        objects.clear();

        std::cout << "Cleaning up physics engine" << std::endl;
    }

}  // namespace Entropy

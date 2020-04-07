
#include "PhysicsEngine.hpp"
namespace Entropy {

    void PhysicsEngine::checkForCollisions(PhysicsObject &obj, vec3 &prePos) {
        PROFILE_FUNCTION();
        for (auto _obj : objects) {
            if (&obj != _obj) {
                switch (obj.collisionType) {
                    case AABBCOLLISION: {
                        CollisionData collision;

                        c2AABBtoAABBManifold(scaleForCollision(obj.getAABB()), scaleForCollision(_obj->getAABB()),
                                             &collision.manifold);

                        collision.collision =
                            c2AABBtoAABB(scaleForCollision(obj.getAABB()), scaleForCollision(_obj->getAABB()));

                        if (collision.collision) {
                            for (int i = 0; i < collision.manifold.count; ++i) {
                                auto p = scaleForRenderering(collision.manifold.contact_points[i]);
                                auto n = (collision.manifold.n);

                                if (debug) {
                                    renderer.renderCircle(vec3(p.x, p.y, 0), 5.0f);
                                    renderer.renderLine(vec3(p.x, p.y, 0), vec3(p.x + n.x, p.y + n.y, 0) * 10.0f);
                                }
                            }
                            obj.collidedLastFrame = true;
                            obj.collide(prePos, _obj, collision);
                            _obj->collide(prePos, &obj, collision);
                        } else {
                            obj.collidedLastFrame = false;
                        }

                        break;
                    }

                    default:
                        break;
                }
            }
        }
    }

    void PhysicsEngine::timeStep(float timeStep) {
        PROFILE_FUNCTION();
        const int collisionPrecision = 1000;

        for (auto obj : objects) {
            if (obj->velocity != vec3(0) && obj->physicsType == ACTIVE) {
                vec3 prePos = obj->getPosition();

                obj->customPrePhysicsStep((float)timeStep);

                obj->velocity.x = floorf(obj->velocity.x * collisionPrecision) / collisionPrecision;
                obj->velocity.y = floorf(obj->velocity.y * collisionPrecision) / collisionPrecision;
                obj->velocity.z = floorf(obj->velocity.z * collisionPrecision) / collisionPrecision;
                obj->velocity = obj->velocity - ((obj->velocity * (float)(obj->friction * 10)) * timeStep);

                checkForCollisions(*obj, prePos);

                obj->setPosition(obj->position + (obj->velocity) * timeStep);

                if (debug) {
                    renderer.renderQuad(obj->getPosition(), obj->boundingBox.width, obj->boundingBox.height, true,
                                        obj->collidedLastFrame ? vec3(0, 1, 0) : vec3(1, 0, 0));
                }
            }
        }
    }

    PhysicsEngine::PhysicsEngine(m_2dRenderer &_renderer, Screen &_screen) : screen(_screen), renderer(_renderer) {}

    PhysicsEngine::~PhysicsEngine() {
        objects.clear();

        std::cout << "Cleaning up physics engine" << std::endl;
    }

}  // namespace Entropy

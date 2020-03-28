
#include "PhysicsEngine.hpp"

namespace Entropy {

    bool PhysicsEngine::AABBIntersectionTest(PhysicsObject *obj1,
                                             PhysicsObject *obj2) {
        return (obj1->getPosition().x - obj1->boundingBox.width / 1 <
                    obj2->getPosition().x + obj2->boundingBox.width / 1 &&
                obj1->getPosition().x + obj1->boundingBox.width / 1 >
                    obj2->getPosition().x - obj2->boundingBox.width / 1 &&
                obj1->getPosition().y - obj1->boundingBox.height / 1 <
                    obj2->getPosition().y + obj2->boundingBox.height / 1 &&
                obj1->getPosition().y + obj1->boundingBox.height / 1 >
                    obj2->getPosition().y - obj2->boundingBox.height / 1);
    }

    CollisionData PhysicsEngine::AABBCollisionTest(PhysicsObject *obj1,
                                                   PhysicsObject *obj2) {
        CollisionData data;
        data.collision = AABBIntersectionTest(obj1, obj2);

        if (data.collision) {
            PhysicsObject *object = new PhysicsObject();

            data.collisionDirection = LEFT;

            object->boundingBox.width = obj2->boundingBox.width - 1.5;
            object->boundingBox.height = obj2->boundingBox.height - 1.5;

            object->setPosition(obj2->getPosition());
            object->setPosition(object->getPosition() +
                                vec3(object->boundingBox.width * 2 + 2, 0, 0));

            if (AABBIntersectionTest(obj1, object)) {
                data.collisionDirection = RIGHT;
            }

            object->setPosition(obj2->getPosition());
            object->setPosition(object->getPosition() +
                                vec3(0, object->boundingBox.width * 2 + 2, 0));

            if (AABBIntersectionTest(obj1, object) &&
                data.collisionDirection != RIGHT) {
                data.collisionDirection = TOP;
            }

            object->setPosition(obj2->getPosition());
            object->setPosition(object->getPosition() -
                                vec3(0, object->boundingBox.width * 2 + 2, 0));

            if (AABBIntersectionTest(obj1, object) &&
                data.collisionDirection != TOP) {
                data.collisionDirection = BOTTOM;
            }

            object->setPosition(obj2->getPosition());
            object->setPosition(object->getPosition() -
                                vec3(object->boundingBox.width * 2 + 2, 0, 0));

            if (AABBIntersectionTest(obj1, object)) {
                data.collisionDirection = LEFT;
            }

            delete object;
        }

        return data;
    }

    void PhysicsEngine::timeStep(float timeStep) {
        GL_LOG("renderLine");

        for (auto obj : objects) {
            if (obj->velocity != vec3(0) && obj->physicsType == ACTIVE) {

                // cout << obj->velocity.x << endl;
                // cout << obj->velocity.y << endl;
                // cout << obj->velocity.z << endl;

                vec3 prePos = obj->getPosition();

                obj->customPrePhysicsStep((float)timeStep);

                obj->velocity.x = floorf(obj->velocity.x * 1000) / 1000;
                obj->velocity.y = floorf(obj->velocity.y * 1000) / 1000;
                obj->velocity.z = floorf(obj->velocity.z * 1000) / 1000;
                obj->velocity =
                    obj->velocity -
                    ((obj->velocity * (float)(obj->friction * 10)) * timeStep);

                // obj->update();

                obj->setPosition(obj->position + (obj->velocity) * timeStep);
                if (obj->physicsType == ACTIVE) {
                    for (auto _obj : objects) {
                        if (obj != _obj) {
                            switch (obj->collisionType) {
                                case AABBCOLLISION: {
                                    CollisionData collision =
                                        AABBCollisionTest(obj, _obj);
                                    if (collision.collision) {
                                        obj->collidedLastFrame = true;
                                        obj->collide(prePos, _obj, collision);
                                        _obj->collide(prePos, obj, collision);
                                    } else {
                                        obj->collidedLastFrame = false;
                                    }

                                    break;
                                }

                                default:
                                    break;
                            }
                        }
                    }
                }

                if (debug) {
                    renderer.renderQuad(
                        obj->getPosition(), obj->boundingBox.width,
                        obj->boundingBox.height, true,
                        obj->collidedLastFrame ? vec3(0, 1, 0) : vec3(1, 0, 0));
                }
            }
        }
    }

    PhysicsEngine::PhysicsEngine(m_2dRenderer &_renderer)
        : renderer(_renderer) {}

    PhysicsEngine::~PhysicsEngine() {
        // for (auto obj : objects)
        //   delete obj;

        objects.clear();

        std::cout << "Cleaning up physics engine" << std::endl;
    }

}  // namespace Entropy

#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "../Graphics/Shapes/Shape.hpp"
#include "collisionData.hpp"

using namespace glm;

enum {
    BOUNDINGBOXCOLLISION,
    AABBCOLLISION,
    POLYGONCOLLISION,
    ACTIVE,
    STATIC
};
namespace Entropy
{

class PhysicsObject
{
protected:
    mat4 modelMatrix;

public:
    string name;
    int collisionType;
    int physicsType = STATIC;
    BoundingBox boundingBox;

    bool collidedLastFrame = false;

    virtual void setPosition(vec3 v) {position = v;}
    virtual vec3 getPosition() {return position;}

    virtual void setModelMatrix(mat4 m) {modelMatrix = m;}
    virtual mat4 getModelMatrix() {return modelMatrix;}

    std::vector<float> vertices;

    double friction = 1;
    vec3 position;
    vec3 velocity;

    virtual void update() {};

    // std::function<void> customPhysicsStep;
    // void (*customPhysicsStep) (double) = nullptr;
    virtual void customPrePhysicsStep (double deltaTime) {};
    virtual void collide(vec3 prePos, PhysicsObject* collided, CollisionData data) {};

    PhysicsObject();
    PhysicsObject(Shape shape) : PhysicsObject() {vertices = shape.vertices;};
    ~PhysicsObject();
};

} // namespace Entropy

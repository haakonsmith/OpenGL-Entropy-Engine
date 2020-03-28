#pragma once

#include "../Graphics/Shapes/Shape.hpp"
#include "CollisionData.hpp"
#include "glm/glm.hpp"
#include <string>
#include <vector>

using namespace glm;

enum
{
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

    PhysicsObject();
    PhysicsObject(Shape shape) : PhysicsObject() { Vertices = shape.Vertices; };
    ~PhysicsObject();
};

} // namespace Entropy

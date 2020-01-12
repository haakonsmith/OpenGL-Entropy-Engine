#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "../Graphics/Shapes/Shape.hpp"

using namespace glm;


namespace Entropy
{

class PhysicsObject
{
protected:
    mat4 modelMatrix;

public:
    virtual void setPosition(vec3 v) {position = v;}
    virtual vec3 getPosition() {return position;}

    virtual void setMVP(mat4 mvp) {MVP = mvp;}
    virtual void setModelMatrix(mat4 m) {modelMatrix = m;}

    virtual mat4 getModelMatrix() {return modelMatrix;}

    glm::mat4 MVP;

    std::vector<float> vertices;

    double friction = 1;
    vec3 position;
    vec3 velocity;

    // std::function<void> customPhysicsStep;
    // void (*customPhysicsStep) (double) = nullptr;
    virtual void customPrePhysicsStep (double deltaTime) {};


    PhysicsObject();
    PhysicsObject(Shape shape) {vertices = shape.vertices;};
    ~PhysicsObject();
};

} // namespace Entropy

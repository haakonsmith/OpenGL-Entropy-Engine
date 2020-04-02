#include <iostream>

#include "Game/GameObject.hpp"
#include "Physics/CollisionData.hpp"

#pragma once

class Bullet : public Entropy::PhysicsObject {
  public:
    float rotation;
    float speed;
    bool shouldDie = false;

    void customPrePhysicsStep(double deltaTime) override;
    inline void collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data) override;

    Bullet(vec3 _position, vec3 direction);
    ~Bullet();
};

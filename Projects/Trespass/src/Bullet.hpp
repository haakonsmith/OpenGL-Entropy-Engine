#include <iosfwd>

#include "Components/Light.hpp"
#include "Game/GameObject.hpp"
#include "Physics/CollisionData.hpp"

#pragma once

class Bullet : public Entropy::PhysicsObject {
  public:
    float rotation;
    float speed;
    bool shouldDie = false;

    shared_ptr<Entropy::Light> light;

    void customPrePhysicsStep(double deltaTime) override;
    inline void collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data) override;

    Bullet(vec3 _position, vec3 direction);
    ~Bullet();
};

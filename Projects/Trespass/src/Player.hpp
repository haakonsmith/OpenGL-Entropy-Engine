#pragma once

#include "Graphics/2dRenderer.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <iostream>
#include <OpenGL/gl3.h>
#include "Game/GameObject.hpp"
#include "Bullet.hpp"
#include "Physics/CollisionData.hpp"

class Player : public Entropy::GameObject {

public:

    Entropy::m_2dRenderer* renderer;
    Entropy::PhysicsEngine* world;

    shared_ptr <Renderable> bullet_Reference;

    vector<shared_ptr<Bullet>> bullets;

    void update() override;

    void customPrePhysicsStep (double deltaTime) override {
        velocity = glm::normalize(velocity) * std::max(std::abs(velocity.x), std::abs(velocity.y));
    }

    void shootBullet();

    void collide(vec3 prePos, Entropy::PhysicsObject* obj, Entropy::CollisionData data) override {
        // velocity = vec3(0);
        setPosition(prePos);
    }

    Player();
    ~Player();
};


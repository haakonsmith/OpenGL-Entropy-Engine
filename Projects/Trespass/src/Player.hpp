#pragma once

#include "Graphics/2dRenderer.hpp"
#include "Physics/PhysicsEngine.hpp"
#include <iostream>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include "Game/GameObject.hpp"
#include "Bullet.hpp"
#include <stdlib.h>
#include "Enemy.hpp"
#include "Physics/CollisionData.hpp"

class Player : public Entropy::GameObject {

public:
    bool shouldCreate = false;

    Entropy::m_2dRenderer* renderer;
    Entropy::PhysicsEngine* world;

    shared_ptr <Renderable> bullet_Reference;
    shared_ptr <Renderable> enemy_Reference;

    shared_ptr <Entropy::RenderInstance<10>> enemy_instance;

    // BufferObject<GL_ARRAY_BUFFER> positionBuffer;
    

    float data[3] = {10,10,0};

    vector<shared_ptr<Bullet>> bullets;
    vector<shared_ptr<Enemy>> enemies;

    void update() override;

    void customPrePhysicsStep (double deltaTime) override {
        velocity = glm::normalize(velocity) * std::max(std::abs(velocity.x), std::abs(velocity.y));
    }

    void shootBullet();
    void createEnemy(const vec3& position) {
        if (shouldCreate) {
            auto enemy = make_shared<Enemy>();
            enemy->setPosition(position);
            enemy->physicsType = ACTIVE;
            enemy->friction = 1;
            enemy->boundingBox.width = 10;
            enemy->boundingBox.height = 10;
            world->addObject(enemy.get());
            enemies.push_back(enemy);
        }
    }

    void collide(vec3 prePos, Entropy::PhysicsObject* obj, Entropy::CollisionData data) override {
        // velocity = vec3(0);
        setPosition(prePos);
    }

    Player();
    ~Player();
};


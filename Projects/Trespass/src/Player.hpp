#pragma once

#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Game/GameObject.hpp"
#include "Graphics/2dRenderer.hpp"
#include "Physics/CollisionData.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Application.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <iosfwd>
#include <stdlib.h>

class Player : public Entropy::GameObject
{

public:
    bool shouldCreate = false;

    Entropy::m_2dRenderer *renderer;
    Entropy::PhysicsEngine *world;

    shared_ptr<Renderable> bullet_Reference;
    shared_ptr<Renderable> enemy_Reference;

    shared_ptr<Entropy::RenderInstance<10>> enemy_instance;
    
    vector<shared_ptr<Bullet>> bullets;
    vector<shared_ptr<Enemy>> enemies;

    void update() override;

    void customPrePhysicsStep(double deltaTime) override
    {
        data.velocity = glm::normalize(data.velocity) * std::max(std::abs(data.velocity.x), std::abs(data.velocity.y));
    }

    void shootBullet();
    void createEnemy(const vec3 &position)
    {
        if (shouldCreate)
        {
            auto enemy = make_shared<Enemy>();
            enemy->setPosition(position);
            enemy->data.physicsType = ACTIVE;
            enemy->data.friction = 1;
            enemy->collider.boundingBox.width = 10;
            enemy->collider.boundingBox.height = 10;
            world->addObject(enemy.get());
            enemies.push_back(enemy);
        }
    }

    void collide(vec3 prePos, Entropy::PhysicsObject *obj, Entropy::CollisionData data) override
    {
        // auto iff = [](int n) 
        // { 
        //     if (n == 0) 
        //         return 1;
        //     else
        //         return 0;
        // };
        preventIntersection(data);
        // velocity = velocity * vec3(iff(data.manifold.n.x), iff(data.manifold.n.y), 1);
        // setPosition(getPosition()  + vec3(data.manifold.n.x*-1, data.manifold.n.y*-1, 0));
    }

    Player();
    ~Player();
};

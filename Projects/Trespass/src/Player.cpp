#include "Player.hpp"

void Player::update() {
        for (size_t i = 0; i < bullets.size(); i++)
        {
            renderer->transform(bullets[i]);

            bullets[i]->setPosition(bullets[i]->getPosition());
            if (bullets[i]->shouldDie)
            {
                renderer->removeRenderable(bullets[i]);
                world->removeObject(bullets[i]);
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
            }
            
        }
        
    }

void Player::shootBullet() {
    vec3 direction = vec3(vec3(glm::rotate(
      mat4(1.0f),
      glm::radians((float)rotation - 45),
      glm::vec3(0.0f, 0.0f, 1.0f))*vec4(1,0,0,0)));

    Bullet* bullet = new Bullet(getPosition(), direction, Entropy::Rectangle());

    bullet->rotation = rotation;

    bullet->physicsType = ACTIVE;
    bullet->friction = 0;

    bullet->setTexture(renderer->loadTexture("floor.png"));
    bullet->setScale(vec3(10));

    // bullet->velocity = vec3(20,20,0);

    renderer->addRenderable(bullet);
    world->addObject(bullet);

    bullets.push_back(bullet);
}

Player::Player() : Entropy::GameObject() {
    PhysicsObject::name = "Player";
    Renderable::name = "Player";
}

Player::~Player() {

}
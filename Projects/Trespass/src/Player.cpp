#include "Player.hpp"
#include "Graphics/Shapes/Shape.hpp"

void Player::update() {
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
        for (size_t i = 0; i < bullets.size(); i++)
        {

            bullets[i]->setPosition(bullets[i]->getPosition());
            if (bullets[i]->shouldDie)
            {
                // renderer->removeRenderable(bullets[i].get());
                world->removeObject(bullets[i].get());
                // delete bullets[i];
                bullets.erase(bullets.begin() + i);
            }

            bullet_Reference->setPosition(bullets[i]->getPosition());
            bullet_Reference->rotation = bullets[i]->rotation;

            renderer->transform(bullet_Reference.get());
            renderer->render(bullet_Reference.get());

            
        }
        
    }

void Player::shootBullet() {
    if (bullet_Reference->cleanVBO)
    {
        renderer->buffer(bullet_Reference.get());
        bullet_Reference->cleanVBO = false;
        bullet_Reference->setTexture(renderer->loadTexture("bullet.png"));
        bullet_Reference->setScale(vec3(10,12,0));
    }

    vec3 direction = vec3(vec3(glm::rotate(
      mat4(1.0f),
      glm::radians((float)rotation - 45),
      glm::vec3(0.0f, 0.0f, 1.0f))*vec4(1,0,0,0)));

    shared_ptr<Bullet> bullet = make_shared<Bullet>(getPosition(), direction, Entropy::Rectangle());

    bullet->rotation = rotation + 45;

    bullet->physicsType = ACTIVE;
    bullet->friction = 0;

    // bullet->vertexBufferID = bullet_Reference->vertexBufferID;
    // bullet->UVBufferID = bullet_Reference->UVBufferID;
    // bullet->texture = bullet_Reference->texture;
    // bullet->cleanVBO = bullet_Reference->cleanVBO;


    

    

    // bullet->velocity = vec3(20,20,0);

    // renderer->addRenderable(bullet.get());
    world->addObject(bullet.get());

    bullets.push_back(bullet);
}

Player::Player() : Entropy::GameObject() {
    PhysicsObject::name = "Player";
    Renderable::name = "Player";

    bullet_Reference = make_shared<Renderable>(Entropy::Rectangle());

}

Player::~Player() {

}
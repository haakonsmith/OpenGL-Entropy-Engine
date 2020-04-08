#include "Player.hpp"

#include "Graphics/Shapes/Shape.hpp"

void Player::update() {
    glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i]->setPosition(bullets[i]->getPosition());
        if (bullets[i]->shouldDie) {
            world->removeObject(bullets[i].get());
            bullets.erase(bullets.begin() + i);
        }

        bullet_Reference->setPosition(bullets[i]->getPosition());
        bullet_Reference->transform.rotation = bullets[i]->rotation;

        bullet_Reference->transform.compute();
        renderer->render(bullet_Reference.get());
    }

    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]->shouldDie) {
            world->removeObject(enemies[i].get());
            enemies.erase(enemies.begin() + i);
        }
        
        enemies[i]->data.velocity = normalize(getPosition() - enemies[i]->getPosition()) * 100.0f;
    }

    if (enemies.size() != 0) {
        std::array<vec3, 10> PosData;

        for (size_t i = 0; i < enemies.size(); i++) {
            if (i < PosData.size()) { PosData[i] = Entropy::App::screen.localSpace(enemies[i]->getPosition()); }
        }

        enemy_instance->update(PosData);
        renderer->renderInstance<10>(*enemy_instance, enemies.size());
    }
}

void Player::shootBullet() {
    if (bullet_Reference->cleanVBO) {
        renderer->buffer(bullet_Reference.get());
        bullet_Reference->cleanVBO = false;
        bullet_Reference->setTexture(renderer->loadTexture("bullet.png"));
        bullet_Reference->setScale(vec3(10, 12, 0));

        enemy_Reference->setTexture(renderer->loadTexture("img_test.png"));
        renderer->buffer(enemy_Reference.get());
        enemy_Reference->cleanVBO = false;
        enemy_Reference->setPosition(vec3(320, 240, 0));
        enemy_Reference->setScale(vec3(10, 12, 0));
        enemy_Reference->transform.compute();
    }

    vec3 direction = vec3(vec3(
        glm::rotate(mat4(1.0f), glm::radians((float)transform.rotation - 45), glm::vec3(0.0f, 0.0f, 1.0f)) * vec4(1, 0, 0, 0)));

    shared_ptr<Bullet> bullet = make_shared<Bullet>(getPosition(), direction);

    bullet->rotation = transform.rotation + 45;

    bullet->data.physicsType = ACTIVE;
    bullet->data.friction = 0;

    bullet->collider.boundingBox.width = 10;
    bullet->collider.boundingBox.height = 10;

    world->addObject(bullet.get());

    bullets.push_back(bullet);
}

Player::Player() : Entropy::GameObject() {
    PhysicsObject::name = "Player";
    Renderable::name = "Player";

    bullet_Reference = make_shared<Renderable>(Entropy::Rectangle());
    enemy_Reference = make_shared<Renderable>(Entropy::Rectangle());

    enemy_instance = renderer->getRenderInstance<10>(enemy_Reference);

    GL_LOG("create ");

    
    enemy_Reference->shader = make_shared<Entropy::Shader>("shaders/Instance.vertexshader", "shaders/Instance.fragmentshader");
}

Player::~Player() {}
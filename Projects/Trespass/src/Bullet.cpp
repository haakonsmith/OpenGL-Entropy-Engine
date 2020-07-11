#include "Bullet.hpp"

#include "Player.hpp"

Bullet::Bullet(vec3 _position, vec3 direction) {
    data.velocity = direction * 700.0f;
    setPosition(_position + direction * 20.0f);
    PhysicsObject::name = "Bullet";

    light = shared_ptr<Entropy::Light>(new Entropy::Light());

    light->position = _position;
    light->intensity = 1;
    light->colour = vec3(0, 1, 0);
}

void Bullet::collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data) {
    // LOG("COLLSION)");
    if (!collided->is<Player>()) shouldDie = true;
}

void Bullet::customPrePhysicsStep(double deltaTime) {
    // velocity = glm::normalize(velocity) * std::max(std::abs(velocity.x), std::abs(velocity.y));
    light->position = getPosition();
}

// Bullet::Bullet(vec3 _position, vec3 direction)

Bullet::~Bullet() {}
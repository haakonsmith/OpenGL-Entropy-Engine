#include "Bullet.hpp"

Bullet::Bullet(vec3 _position, vec3 direction, Entropy::Shape sp) : PhysicsObject(sp)
{
    velocity = direction * 700.0f;
    setPosition(_position + direction * 13.0f);
    PhysicsObject::name = "Bullet";
}

void Bullet::collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data){
    // std::cout << ("COLLIDE BULLET\n") ;
    shouldDie = true;
}

void Bullet::customPrePhysicsStep(double deltaTime)
{
    // velocity = glm::normalize(velocity) * std::max(std::abs(velocity.x), std::abs(velocity.y));
}

// Bullet::Bullet(vec3 _position, vec3 direction)

Bullet::~Bullet()
{
}
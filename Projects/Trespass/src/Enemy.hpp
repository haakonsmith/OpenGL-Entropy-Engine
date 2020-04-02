#pragma once

class Bullet;
class Enemy : public Entropy::PhysicsObject {
  public:
    bool shouldDie = false;
    void collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data){
      if (collided->is<Bullet>())
        shouldDie = true; 
      else
        preventIntersection(data);
      
  }
};
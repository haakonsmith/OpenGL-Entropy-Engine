
class Enemy : public Entropy::PhysicsObject {
  public:
    bool shouldDie = false;
    void collide(vec3 prePos, PhysicsObject* collided, Entropy::CollisionData data){
      if (typeid(*this) != typeid(*collided))
        shouldDie = true; 
      // std::cout << typeid(Enemy).name() << std::endl;
      // std::cout << typeid(*this).name() << std::endl;
      // std::cout << typeid(*collided).name() << std::endl;
      
  }
};
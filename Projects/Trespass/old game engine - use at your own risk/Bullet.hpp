
class Bullet
{
private:
  /* data */
public:
  float x, y, rotation;
  int walls_hit;
  float width, height;
  float xV, yV;

  bool buffered;

  std::unique_ptr<GameObject> Game_object;
  Bullet(double rotation, float playerX, float playerY, std::unique_ptr<GameObject> Gameobject);

  ~Bullet();
  void show();
  float boundary(int dire);

  void buffer();
  void collide(int side);
  void draw();
  void transform(float rotation, glm::vec3 translation, glm::vec3 scale, float rot);
};

Bullet::Bullet(double my_rotation, float playerX, float playerY, std::unique_ptr<GameObject> Gameobject)
{
  // random device class instance, source of 'true' randomness for initializing random seed
  std::random_device rd;

  // Mersenne twister PRNG, initialized with seed from previous random device instance
  std::mt19937 gen(rd());

  double mean = 0;
  double variance = -5;

  // instance of class std::normal_distribution with specific mean and stddev
  std::normal_distribution<float> d(mean, variance);

  rotation = glm::radians(my_rotation) * -1 + glm::radians(d(gen));
  x = playerX;
  y = playerY;
  walls_hit =0;

  xV = cos(rotation) * 10;
  yV = sin(rotation) * 10;

  Game_object = std::move(Gameobject);
  // glGenVertexArrays(1, &VertexArrayID);
  // glBindVertexArray(VertexArrayID);
  std::cout << glm::degrees(rotation) << "<< rotation\n";
}

Bullet::~Bullet()
{
}

void Bullet::collide(int side){
  // x = x - cos(rotation) * 20;
  // y = y - sin(rotation) * 20;
  // rotation = rotation * -1;
  // rotation = rotation - glm::radians(90.0f);

  // x = x - xV * 1;
  // y = y - yV * 1;
  
  switch (side)
  {
    case RIGHT:
      x = x - xV * 2;
      xV = xV * -1;
      break;
  
    case LEFT:
      x = x - xV * 2;
      xV = xV * -1;
      break;
    case TOP:
      y = y - yV * 2;
      yV = yV * -1;
      break;
    case BOTTOM:
      y = y - yV * 2;
      yV = yV * -1;
      break;
    default:
      break;
  }
  
}

float Bullet::boundary(int dire){

  switch (dire)
  {
  case UP:
    return y + (height / 2);
    break;
  case DOWN:
    return y + (height / 2 * -1);
    break;
  case LEFT:
    return x + (width / 2 * -1);
    break;
  case RIGHT:
    return x + (width / 2 * 1);
    break;

  default:
    break;
  }
}

void Bullet::transform(float rotation, glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float rot = 45.0)
{
  Game_object->transform(rotation, translation, scale, rot);
}

void Bullet::draw()
{
  Game_object->draw();
}

void Bullet::show()
{
  Game_object->transform(rotation-47*-1, glm::vec3(x, y, 1), glm::vec3(0.1f, 0.3f, 0.5f));
  Game_object->draw();

  // x = x + 1;
  // y = y + tan(rotation*10);

  x = x + xV;
  y = y + yV;
  

  // << sin(rotation) << "\n";
}

void Bullet::buffer(){
  // Game_object->buffer();
  buffered = true;
}
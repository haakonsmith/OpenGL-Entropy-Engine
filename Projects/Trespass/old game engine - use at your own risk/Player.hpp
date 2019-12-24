
class Player
{
private:
  std::vector<std::unique_ptr<Bullet>> bullets;

public:
  std::unique_ptr<GameObject> game_object;
  std::shared_ptr<GameObject> bullet_object;
  GLuint shader;
  float x, y, speed;

  double get_x();
  double get_y();

  glm::vec2 vec_rotation;

  float rotation;
  void move(short int dire);
  int boundary(int move);

  void draw();

  std::shared_ptr<Room> room;

  void drawBullets();

  void shoot(GLuint vbo);

  void buffer();

  void transform(float rotation, glm::vec3 translation, glm::vec3 scale, float rot);

  Player(std::unique_ptr<GameObject> my_object, std::shared_ptr<Room> my_room, int my_x, int my_y);
};

int Player::boundary(int dire)
{

  switch (dire)
  {
    case UP:
      // Cxp
      return y + (game_object->oHeight / 2);
      break;
    case DOWN:
      return y + (game_object->oHeight / 2 * -1);
      break;
    case LEFT:
      return x + (game_object->oWidth / 2 * -1);
      break;
    case RIGHT:
      return x + (game_object->oWidth / 2 * 1);
      break;

    default:
      break;
    }
  }

  void Player::move(short int dire)
  {
    int count;

    std::vector<objectLite> walls = room->objectsLite();

    std::vector<objectLite> objects;

    count = objects.size();

    objects.insert(objects.end(), walls.begin(), walls.end());
    

    switch (dire)
    {
    case LEFT:
    {

      auto object = game_object->sharedLite();
      object->X = object->X - 1 * speed;

      if (!checkInside(object, objects))
      {
        x = x - 1 * speed;
      }
      break;
      }
    case RIGHT:
    {
      auto object = game_object->sharedLite();
      object->X = object->X + 1 * speed;

      if (!checkInside(object, objects))
      {
        x = x + 1 * speed;
      }
      break;
      }
    case UP:
    {
      auto object = game_object->sharedLite();
      object->Y = object->Y + 1 * speed;

      if (!checkInside(object, objects))
      {
        y = y + 1 * speed;
      }
      break;
      }
    case DOWN:
    {
      auto object = game_object->sharedLite();
      object->Y = object->Y - 1 * speed;

      if (!checkInside(object, objects))
      {
        y = y - 1 * speed;
      }
      break;
      }
    case ROTATE:
    {
      rotation++;
      break;
      }
    }
  }

Player::Player(std::unique_ptr<GameObject> my_object, std::shared_ptr<Room> my_room, int my_x = SCREEN_WIDTH / 2, int my_y = SCREEN_HEIGHT / 2)
{
  room = my_room;
  game_object = std::move(my_object);
  x = my_x;
  y = my_y;
  rotation = 0;
}

void Player::draw()
{
  game_object->draw();
}

void Player::buffer()
{
  game_object->buffer();
  // printf("game");
}

void Player::transform(float rotation = 0, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float rot = 45.0)
{
  // so far rot is not necceary
  if (rotation != 0)
  {
    // game_object->transform(rotation, glm::vec3(1.0f, 1.0f, 1.0f), scale, rot);
    // game_object->transform(0, translation, scale, rot);
    game_object->transform(rotation, translation, scale, rot);
  }
  else
  {
    game_object->transform(rotation, translation, scale, rot);
  }
}

void Player::shoot(GLuint vbo)
{
  std::unique_ptr<GameObject> bullet_ob(new GameObject(glm::vec3(1, 1, 1), 0,0));
  bullet_ob->add_shape("tri", TRIANGLE, shader, vbo);

  // create a bullet smart ptr
  std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(double(rotation)*-1, x, y, std::move(bullet_ob));
  // bullet->buffer();
  bullets.push_back(std::move(bullet));
}

void Player::drawBullets()
{
  int count;

  std::vector<c2LitePoly> walls = room->Polys();

  std::vector<c2LitePoly> objects;

  count = objects.size();

  objects.insert(objects.end(), walls.begin(), walls.end());


  int side;

  if (!bullets.empty()) {
  
    for (size_t i = 0; i < bullets.size(); i++)
    {
      bool collision = false;
      // std::cout << objects[i].X << objects[i].Y << "   <<<<<  x and y\n";
      // std::cout << objects.size() << "\n";
       c2Poly bulletPoly = bullets[i]->Game_object->Poly();


       c2x bulletPos = c2Transform(c2V(bullets[i]->x, bullets[i]->y), 10.0f);

       if (!bullets[i]->buffered)
       {
         bullets[i]->buffer();
       }

       for(size_t i = 0; i < count; i++)
       {
         c2x objectsI_transform = c2Transform(c2V(objects[i].x, objects[i].y), objects[i].size);
         if (c2PolytoPoly(&bulletPoly, &bulletPos, &objects[i].poly, &objectsI_transform))
         {
          std::printf("\t\tcollide\n");
          collision = true;
         }
       }
       
       //   std::shared_ptr<objectLite> liteObject = bullets[i]->Game_object->sharedLite();

       

        if (!collision)
        {
          bullets[i]->show();
        }
        else
        {
          // collisonSide(liteObject, objects[0]);

          // std::cout << "<< collison side  \n\n";
          // bullets.erase(bullets.begin() + i);

          if (bullets[i]->walls_hit > 0) {
            bullets.erase(bullets.begin() + i);
          }

          else
          {
            printf("side");

            bullets[i]->walls_hit++;
            bullets[i]->collide(side);
            // bullets[i]->rotation = bullets[i]->rotation *-1;
            bullets[i]->show();
          }

          // bullets[i]->show();
          std::printf("bullet collison");
        }
      }
  }
}


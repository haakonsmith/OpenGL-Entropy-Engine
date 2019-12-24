

struct objectLite
{
  int X;
  int Y;
  int sizeX;
  int sizeY;
  int type;
  objectLite(int type, int x, int y, int sizex, int sizey);
};

objectLite::objectLite(int Type, int x, int y, int sizex = 5, int sizey = 5)
{
  X = x;
  Y = y;
  sizeX = sizex;
  sizeY = sizey;
  type = Type;
}


struct c2LitePoly
{
  int x, y;
  int size;
  c2Poly poly;
};

// int b2collide(b2LitePoly object, b2LitePoly object2)
// {
//   b2PolygonShape
// }

int TEST_SIDE_DO_NOT_USE;
int collisionSide(std::shared_ptr<objectLite> object, objectLite object2, int &side = TEST_SIDE_DO_NOT_USE)
{

  // int count = collisons.size();

  // for(size_t i = 0; i < count; i++)
  // {

  object2.X = object2.X - object2.sizeX / 2;
  object2.Y = object2.Y - object2.sizeY / 2;



  float player_bottom = object->Y + object->sizeY / 2;
  float tiles_bottom = object2.Y + object2.sizeY / 2;
  float player_right = object->X + object->sizeX / 2;
  float tiles_right = object2.X + object2.sizeX / 2;

  float b_collision = tiles_bottom - object->Y;
  float t_collision = player_bottom - object2.Y;
  float l_collision = player_right - object2.X;
  float r_collision = tiles_right - object->X;

  // float player_bottom = object2.Y + object2.sizeY / 2;
  // float tiles_bottom = object->Y + object->sizeY  / 2;
  // float player_right = object2.X + object2.sizeX / 2;
  // float tiles_right = object->X + object->sizeX  / 2;

  // float b_collision = tiles_bottom - object2.Y;
  // float t_collision = player_bottom - object->Y;
  // float l_collision = player_right - object->X;
  // float r_collision = tiles_right - object2.X;

  if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision)
  {
    std::printf("bottom");
    side = BOTTOM;
    // return TOP;
    //Top collision
  }
  if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)
  {
    std::printf("top");
    side = TOP;
    // return BOTTOM;
    //bottom collision
  }
  if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
  {
    std::printf("right");
    side = LEFT;
    // return RIGHT;
    //Left collision
  }
  if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision)
  {
    std::printf("right");
    side = RIGHT;
    // return LEFT;
    //Right collision
  }
  // }
  return 1;
}

bool checkInside(std::shared_ptr<objectLite> object, std::vector<objectLite> collisions, int &side = TEST_SIDE_DO_NOT_USE)
{
  
  int count = collisions.size();
  for (size_t i = 0; i < collisions.size(); i++)
  {
    // std::cout << "object: X->" << object->X << ", Y->" << object->Y << ", X_size->" << object->sizeX << ", Y_size->" << object->sizeY << "\n";
    // std::cout << "collisons[" << i << "]: X->" << collisions[i].X << ", Y->" << collisions[i].Y << ", X_size->" << collisions[i].sizeX << ", Y_size->" << collisions[i].sizeY << "\n";

    if (
        object->X + object->sizeX * 0.5 < collisions[i].X + collisions[i].sizeX * 0.5 
        &&
        object->X - object->sizeX * 0.5 > collisions[i].X - collisions[i].sizeX * 0.5
        )
    {
      if (
          object->Y + object->sizeY * 0.5 < collisions[i].Y + collisions[i].sizeY * 0.5 
          &&
          object->Y - object->sizeY * 0.5 > collisions[i].Y - collisions[i].sizeY * 0.5
          )
      {
        collisionSide(object, collisions[i], side);
        return true;
      }
    }
  }

  return false;
}


#ifndef MOVE
#define MOVE

enum MOVE
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ROTATE
};

#endif

class Room
{
private:
  
public:
  int sizeX, sizeY;

  std::vector<std::unique_ptr<GameObject>> items;
  std::map<std::string, int> objects_map;

  std::vector<objectLite> walls;
  std::vector<c2LitePoly> polyWalls;

  Room(int sizeX, int sizeY);

  void draw();

  void buffer();

  void transform(std::string name, float rotation, glm::vec3 translation, glm::vec3 scale);

  void add(std::string name, std::unique_ptr<GameObject> obj);

  std::vector<objectLite> objectsLite();
  std::vector<c2LitePoly> Polys();

  bool check(int xTwo, int yTwo, int oTwoWidth, int oTwoHeight);

  int boundary(int dire);
};

std::vector<c2LitePoly> Room::Polys()
{
  std::vector<c2LitePoly> polys = polyWalls;


  for (size_t i = 0; i < items.size(); i++)
  {
    polys.push_back(items[i]->LitePoly());
  }

  return polys;
}

Room::Room(int sizeX_new, int sizeY_new)
{
  sizeX = sizeX_new;
  sizeY = sizeY_new;

  // b2PolygonShape poly;
  // poly

  
  c2LitePoly top_wallLite;
  top_wallLite.x = sizeX / 2;
  top_wallLite.y = sizeX;
  top_wallLite.size = sizeX;
  top_wallLite.poly = createPoly(SQUARE);

  polyWalls.push_back(top_wallLite);

  // walls.push_back(objectLite(
  //     SQUARE, sizeX / 2, sizeY + 100, sizeX + 100, 200));
  // walls.push_back(objectLite(
  //     SQUARE, sizeX / 2, 0 - 100, sizeX + 100, 200));

  // walls.push_back(objectLite(
  //     SQUARE, sizeX + 100, sizeY / 2, 200, sizeY + 100));
  // walls.push_back(objectLite(
  //     SQUARE, 0 - 100, sizeY / 2, 200, sizeY + 300));
}

void Room::add(std::string name, std::unique_ptr<GameObject> obj)
{
  items.push_back(std::move(obj));
  objects_map[name] = items.size() - 1;
}

bool Room::check(int xTwo, int yTwo, int oTwoWidth, int oTwoHeight)
{
  int count = items.size();
  for(size_t i = 0; i < count; i++)
  {
    if (!items[i]->checkCollide(xTwo, yTwo, oTwoWidth, oTwoHeight))
    {
      return false;
    }
  }
  return true;
}

int Room::boundary(int dire){
  switch (dire)
  {
  case UP:
    // Cxp
    return sizeY;
    break;
  case DOWN:
    return 0;
    break;
  case LEFT:
    return 0;
    break;
  case RIGHT:
    return sizeX;
    break;

  default:
    break;
  }
}

void Room::transform(std::string name, float rotation = 0, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)){
  
  if (name == "__all") {
    int count = items.size();
    for(size_t i = 0; i < count; i++)
    {
      items[i]->transform(rotation, translation, scale);
    }
    
  }
  
  else
  {
    items[objects_map[name]]->transform(rotation, translation, scale);
  }
  
}

void Room::draw()
{
  int count = items.size();
  for (size_t i = 0; i < count; i++)
  {
    items[i]->draw();
  }
}


void Room::buffer(){
  int count = items.size();
  for (size_t i = 0; i < count; i++)
  {
    items[i]->buffer();
  }
}

std::vector<objectLite> Room::objectsLite(){
  std::vector<objectLite> objs = walls;

  
  for(size_t i = 0; i < items.size(); i++)
  {
    objs.push_back(items[i]->Lite());
  }

  return objs;
}


class GameObject
{
private:
  

public:
  glm::vec3 XYZ;
  int oWidth;
  int oHeight;

  std::vector<std::unique_ptr<ShapeBase>> shapes;
  std::unique_ptr<ShapeBase> shape;
  std::map<std::string, int> shapes_map;

  void add_shape(std::string name, int new_shape, GLuint shader_program_id, GLuint init_memory_type);

  void draw();

  bool checkCollide(int xTwo, int yTwo, int oTwoWidth, int oTwoHeight);

  void transform(float rotation, glm::vec3 translation, glm::vec3 scale, float rot);

  // void model(std::string name,  float rotation, glm::vec3 translation, glm::vec3 scale);

  int Shape;

  void buffer();

  objectLite Lite();

  std::shared_ptr<objectLite> sharedLite();

  c2Poly Poly();
  c2LitePoly LitePoly();

  std::shared_ptr<c2Poly> sharedPoly();

  GameObject(glm::vec3 start, int height, int width, int init_Shape);
  ~GameObject();
};

void GameObject::add_shape(std::string name, int new_shape, GLuint shader_program_id, GLuint vbo )
{
  std::unique_ptr<ShapeBase> shape = ShapeFactory::createShape(shader_program_id, vbo, new_shape);
  shapes.push_back(std::move(shape));
  shapes_map[name] = shapes.size()-1;
}

GameObject::GameObject(glm::vec3 start, int height = 10, int width = 10, int init_Shape = TRIANGLE)
{
  XYZ = start;
  oWidth = width;
  oHeight = height;
  Shape = init_Shape;
}

bool GameObject::checkCollide(int xTwo, int yTwo, int oTwoWidth, int oTwoHeight)
{
  if (XYZ[0] + oWidth < xTwo || XYZ[0] > xTwo + oTwoWidth)
  {
    return false;
  }
  if (XYZ[1] + oHeight < yTwo || XYZ[1] > yTwo + oTwoHeight)
  {
    return false;
  }
  return true;
}

objectLite GameObject::Lite()
{
  objectLite obj(SQUARE, XYZ[0], XYZ[1], oWidth, oHeight);

  return obj;
}

std::shared_ptr<objectLite> GameObject::sharedLite()
{
  std::shared_ptr<objectLite> obj = std::make_shared<objectLite>(SQUARE, XYZ[0], XYZ[1], oWidth, oHeight);
  // obj->sizeX = oWidth;
  // obj->sizeY = oHeight;
  // obj->X = XYZ[0];
  // obj->Y = XYZ[1];
  return obj;
}

GameObject::~GameObject()
{
}

c2Poly GameObject::Poly()
{
  return createPoly(Shape);
}

c2LitePoly GameObject::LitePoly()
{
  c2LitePoly poly;

  poly.x = XYZ[0];
  poly.y = XYZ[1];
  poly.size = oWidth;
  poly.poly = Poly();

  return poly;
}

void GameObject::draw()
{
  int count = shapes.size();
  if (!shapes.empty())
  {
    for (size_t i = 0; i < count; i++)
    {
      shapes[i]->draw();
    }
  }
}

void GameObject::transform(float rotation = 0, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float rot = 45.0)
{
  XYZ = translation;
  int count = shapes.size();
  for (size_t i = 0; i < count; i++)
  {
    shapes[i]->transform(rotation, translation, scale, rot);
  }
}

void GameObject::buffer(){
  // TODO: fix buffering
  int count = shapes.size();

  // shapes[0]->draw();
  if (!shapes.empty()){
    for (size_t i = 0; i < count; i++)
    {
      shapes[i]->buffer();
      // std::cout<<"test";
    }
  }
  return;
}
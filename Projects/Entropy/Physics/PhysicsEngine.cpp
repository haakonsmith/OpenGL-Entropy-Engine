
#include "PhysicsEngine.hpp"

namespace Entropy
{
float PhysicsEngine::distToNearestPoint(vec3 point)
{

  // vector<float> distances;

  float minDist = 10000000;

  // distances.reserve(objects.size() * 3);

  for (auto obj : objects)
  {
    for (size_t i = 0; i < obj->vertices.size(); i += 3)
    {
      vec3 point2 = vec3(obj->getModelMatrix() * vec4(vec3(obj->vertices[i], obj->vertices[i + 1], obj->vertices[i + 2]), 1));

      // distances.emplace_back(distance(point, point2));
      minDist = std::min(minDist, distance(point, point2));
    }
  }

  return minDist;

  // return *min_element(distances.begin(), distances.end());
}

float PhysicsEngine::distToNearestPoint(vec3 point, PhysicsObject *_obj)
{

  float minDist = 10000000;

  for (auto obj : objects)
  {
    if (obj != _obj)
    {

      for (size_t i = 0; i < obj->vertices.size(); i += 3)
      {
        vec3 point2 = vec3(obj->getModelMatrix() * vec4(vec3(obj->vertices[i], obj->vertices[i + 1], obj->vertices[i + 2]), 1));

        minDist = std::min(minDist, distance(point, point2));
      }
    }
  }

  return minDist;
}

float PhysicsEngine::distToNearestPoint(PhysicsObject *_obj)
{
  // float distances[_obj->vertices.size()/3];
  vector<float> distances;

  for (size_t i = 0; i < _obj->vertices.size(); i += 3)
  {
    vec3 point = vec3(_obj->getModelMatrix() * vec4(vec3(_obj->vertices[i], _obj->vertices[i + 1], _obj->vertices[i + 2]), 1));

    // distances[i] = distToNearestPoint(point, _obj);
    distances.emplace_back(distToNearestPoint(point, _obj));
  }

  // return *min_element(distances, distances + 3);
  return *min_element(distances.begin(), distances.end());
}

float PhysicsEngine::distToObject(vec3 point, PhysicsObject *_obj)
{

  float minDist = 10000000;

  for (size_t i = 0; i < _obj->vertices.size(); i += 6)
  {
    vec3 p1 = vec3(_obj->getModelMatrix() * vec4(vec3(_obj->vertices[i], _obj->vertices[i + 1], _obj->vertices[i + 2]), 1));
    vec3 p2 = vec3(_obj->getModelMatrix() * vec4(vec3(_obj->vertices[i + 3], _obj->vertices[i + 4], _obj->vertices[i + 5]), 1));

    vec3 x = point;

    auto r = dot((p2 - p1), (x - p1));
    r /= length((p2 - p1));

    float dist;

    if (r < 0)
      dist = length(x - p1);
    else if (r > 1)
      dist = length(p2 - x);
    else
      dist = sqrt(length(x - p1) * length(x - p1) - r * length(p2 - p1) * length(p2 - p1));

    minDist = std::min(dist, minDist);
  }

  //x is the point you're looking for
  return minDist;
}

float PhysicsEngine::distToNearestObject(PhysicsObject *_obj)
{
  float minDist = 1000000000;

  for (auto obj : objects)
  {
    if (obj != _obj)
    {
      for (size_t i = 0; i < _obj->vertices.size(); i += 3)
      {
        vec3 point = vec3(_obj->getModelMatrix() * vec4(vec3(_obj->vertices[i], _obj->vertices[i + 1], _obj->vertices[i + 2]), 1));

        LOG(point.x);
        minDist = std::min(minDist, distToObject(point, obj));
      }
    }
  }

  return minDist;
}

std::vector<float> trim_vector(std::vector<glm::vec3> vecs, int axis)
{
  std::vector<float> new_vector;

  for (int i = 0; i < vecs.size(); i++)
  {
    new_vector.push_back(vecs[i][axis]);
  }

  return new_vector;
}

glm::vec2 furthest_points(std::vector<float> points)
{
  std::vector<float> dist;
  glm::vec2 points_;

  for (int i = 0; i < points.size(); i++)
  {

    dist.push_back(glm::distance(points[0], points[i]));
  }

  // std::cout<<std::endl<<highest_point<<std::endl;

  sort(dist.begin(), dist.end());

  points_ = glm::vec2(points[0], points[0] + dist[dist.size()]);
  std::cout << std::endl
            << points_.x << "\t" << points_.y << std::endl;

  return points_;
}

glm::vec2 weird_dist(glm::vec2 A, glm::vec2 B)
{
  return glm::vec2(
      B.x - A.x,
      B.y - A.x);
}

glm::vec2 weird_min(std::vector<glm::vec3> vector)
{
  auto x_vec = trim_vector(vector, 0);
  auto y_vec = trim_vector(vector, 1);

  return glm::vec2(
      *std::min_element(std::begin(x_vec), std::end(x_vec)),
      *std::min_element(std::begin(y_vec), std::end(y_vec)));
}

glm::vec2 weird_max(std::vector<glm::vec3> vector)
{
  auto x_vec = trim_vector(vector, 0);
  auto y_vec = trim_vector(vector, 1);

  return glm::vec2(
      *std::max_element(std::begin(x_vec), std::end(x_vec)),
      *std::max_element(std::begin(y_vec), std::end(y_vec)));
}

// !!!Broken!!!
bool SAT(PhysicsObject *obj1, PhysicsObject *obj2)
{

  // std::printf("collision\n");
  vector<vec3> sp1_verts;
  for (size_t i = 0; i < obj1->vertices.size(); i += 3)
  {
    sp1_verts.emplace_back(vec3(obj1->getModelMatrix() * vec4(vec3(obj1->vertices[i], obj1->vertices[i + 1], obj1->vertices[i + 2]), 1)));
  }
  vector<vec3> sp2_verts;
  for (size_t i = 0; i < obj2->vertices.size(); i += 3)
  {
    sp2_verts.emplace_back(vec3(obj2->getModelMatrix() * vec4(vec3(obj2->vertices[i], obj2->vertices[i + 1], obj2->vertices[i + 2]), 1)));
  }

  // std::cout<< sp1_verts[2].x;

  std::vector<glm::vec2> axes;

  for (int i = 0; i < sp1_verts.size(); i++)
  {
    glm::vec2 normal;

    // vert1 = sp1_verts[i];

    if (!i + 1 > sp1_verts.size())
    {

      normal = glm::vec2(
          sp1_verts[i + 1].x - sp1_verts[i].x,
          sp1_verts[i + 1].y - sp1_verts[i].y);
    }
    else
    {
      normal = glm::vec2(
          sp1_verts[i + 1 - sp1_verts.size()].x - sp1_verts[i].x,
          sp1_verts[i + 1 - sp1_verts.size()].y - sp1_verts[i].y);
    }

    // std::cout << normal.x;

    axes.push_back(glm::normalize(normal));
  }

  for (int i = 0; i < sp2_verts.size(); i++)
  {
    glm::vec2 normal;

    // vert1 = sp2_verts[i];

    if (!i + 1 > sp2_verts.size())
    {

      normal = glm::vec2(
          sp2_verts[i + 1].x - sp2_verts[i].x,
          sp2_verts[i + 1].y - sp2_verts[i].y);
    }
    else
    {
      normal = glm::vec2(
          sp2_verts[i + 1 - sp2_verts.size()].x - sp2_verts[i].x,
          sp2_verts[i + 1 - sp2_verts.size()].y - sp2_verts[i].y);
    }

    axes.push_back(glm::normalize(normal));
  }

  for (int i = 0; i < axes.size(); i++)
  {
    auto axis = axes[i];

    // std::cout<< axis.x << ",	" <<  axis.y;

    auto C = weird_dist(glm::vec2(obj1->position), glm::vec2(obj2->position));
    auto B = weird_dist(
        weird_min(sp1_verts),
        weird_max(sp1_verts));
    auto A = weird_dist(
        weird_min(sp2_verts),
        weird_max(sp2_verts));

    auto projC = glm::dot(axis, C);
    auto projB = glm::dot(axis, B);
    auto projA = glm::dot(axis, A);

    auto gap = projC - projA + projB;

    if (gap > 0)
    {
      return false;
    }
    // else {}
    // std::cout<<std::endl;
  }

  return true;
}

bool PhysicsEngine::AABBIntersectionTest(PhysicsObject *obj1, PhysicsObject *obj2)
{

  return (obj1->getPosition().x - obj1->boundingBox.width / 1 < obj2->getPosition().x + obj2->boundingBox.width / 1 &&
          obj1->getPosition().x + obj1->boundingBox.width / 1 > obj2->getPosition().x - obj2->boundingBox.width / 1 &&
          obj1->getPosition().y - obj1->boundingBox.height / 1 < obj2->getPosition().y + obj2->boundingBox.height / 1 &&
          obj1->getPosition().y + obj1->boundingBox.height / 1 > obj2->getPosition().y - obj2->boundingBox.height / 1);
}

CollisionData PhysicsEngine::AABBCollisionTest(PhysicsObject *obj1, PhysicsObject *obj2)
{
  CollisionData data;
  data.collision = AABBIntersectionTest(obj1, obj2);

  if (data.collision)
  {
    PhysicsObject *object = new PhysicsObject();

    data.collisionDirection = LEFT;

    object->boundingBox.width = obj2->boundingBox.width - 1.5;
    object->boundingBox.height = obj2->boundingBox.height - 1.5;

    object->setPosition(obj2->getPosition());
    object->setPosition(object->getPosition() + vec3(object->boundingBox.width * 2 + 2, 0, 0));

    if (AABBIntersectionTest(obj1, object))
    {
      data.collisionDirection = RIGHT;
    }

    object->setPosition(obj2->getPosition());
    object->setPosition(object->getPosition() + vec3(0, object->boundingBox.width * 2 + 2, 0));

    if (AABBIntersectionTest(obj1, object) && data.collisionDirection != RIGHT)
    {
      data.collisionDirection = TOP;
    }

    object->setPosition(obj2->getPosition());
    object->setPosition(object->getPosition() - vec3(0, object->boundingBox.width * 2 + 2, 0));

    if (AABBIntersectionTest(obj1, object) && data.collisionDirection != TOP)
    {
      data.collisionDirection = BOTTOM;
    }

    object->setPosition(obj2->getPosition());
    object->setPosition(object->getPosition() - vec3(object->boundingBox.width * 2 + 2, 0, 0));

    if (AABBIntersectionTest(obj1, object))
    {
      data.collisionDirection = LEFT;
    }

    delete object;
  }

  return data;
}

void PhysicsEngine::timeStep(float timeStep)
{
  GL_LOG("renderLine");

  // LOG(renderer);
  for (auto obj : objects)
  {

    if (obj->velocity != vec3(0) && obj->physicsType == ACTIVE)
    {
      // LOG(obj->getPosition().x);

      // cout << obj->velocity.x << endl;
      // cout << obj->velocity.y << endl;
      // cout << obj->velocity.z << endl;

      vec3 prePos = obj->getPosition();

      obj->customPrePhysicsStep((float)timeStep);

      obj->velocity.x = floorf(obj->velocity.x * 1000) / 1000;
      obj->velocity.y = floorf(obj->velocity.y * 1000) / 1000;
      obj->velocity.z = floorf(obj->velocity.z * 1000) / 1000;
      obj->velocity = obj->velocity - ((obj->velocity * (float)(obj->friction * 10)) * timeStep);

      // obj->update();

      // LOG(distToNearestObject(obj));
      // LOG(obj->position.x);
      obj->setPosition(obj->position + (obj->velocity) * timeStep);
      // LOG(obj->position.x);
      if (obj->physicsType == ACTIVE)
      {

        for (auto _obj : objects)
        {
          if (obj != _obj)
          {

            switch (obj->collisionType)
            {
            case AABBCOLLISION:
            {

              CollisionData collision = AABBCollisionTest(obj, _obj);
              if (collision.collision)
              {
                obj->collidedLastFrame = true;
                obj->collide(prePos, _obj, collision);
              }
              else
              {
                obj->collidedLastFrame = false;
              }

              break;
            }

            default:
              break;
            }
          }
        }
      }

      if (debug)
      {
        renderer.renderQuad(obj->getPosition(), obj->boundingBox.width, obj->boundingBox.height, true, obj->collidedLastFrame ? vec3(0, 1, 0) : vec3(1, 0, 0));
      }
    }
  }
}

PhysicsEngine::PhysicsEngine(m_2dRenderer &_renderer) : renderer(_renderer)
{
}

PhysicsEngine::~PhysicsEngine()
{
  // for (auto obj : objects)
  //   delete obj;

  objects.clear();

  std::cout << "Cleaning up physics engine" << std::endl;
}

} // namespace Entropy

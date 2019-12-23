double vec_length(glm::vec2 vector)
{
  return sqrt(glm::dot(vector, vector));
}

double angle_between(glm::vec2 vec_a, glm::vec2 vec_b)
{

  double dot = glm::dot(vec_a, vec_b);
  double val = dot / (vec_length(vec_a) * vec_length(vec_b));

  if (val > 1.0)
  {
    return 0.0;
  }
  else
  {
    return acos(val);
  }
}

glm::vec2 point_vec(glm::vec2 point1, glm::vec2 point2)
{
  glm::vec2 vec = glm::vec2(
      point2.x - point1.x,
      point2.y - point1.y);
  return vec;
}
double to_positive_angle(double angle)
{
  // angle = fmod(angle, 360);
  if (angle < 0)
    angle += 360;
  return angle;
}

double angle_between_vectors(float x1, float y1, float x2, float y2)
{
  double player_rotation = 30 * PI / 180;

  glm::vec2 player_point = glm::vec2(x1, y1);
  glm::vec2 mouse_point = glm::vec2(x2, (y2 - SCREEN_HEIGHT) * -1);
  std::cout << mouse_point.x << ",  " << mouse_point.y << "  <<< mouse x, y\n";

  glm::vec2 mouse_vector = point_vec(player_point, mouse_point);
  std::cout << mouse_vector.x << ",  " << mouse_vector.y << "  <<< mouse vector x, y\n";
  // std::cout << mouse_point.x << ",  " << mouse_point.y << "  <<< mouse x, y\n";

  glm::vec2 player_trig = glm::vec2(
      cos(player_rotation) + player_point.x,
      sin(player_rotation) + player_point.y);

  glm::vec2 player_vector = point_vec(player_point, player_trig);
  return to_positive_angle(atan2(player_point.y, player_point.x) - atan2(mouse_point.y, mouse_point.x) * 180 / PI);
  // return angle_between(mouse_vector, player_vector) + player_rotation;
}
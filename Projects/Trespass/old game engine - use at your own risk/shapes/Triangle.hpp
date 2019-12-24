

class Triangle : public ShapeBase
{

public:
  Triangle(GLuint shader_program_id, GLuint init_memory_type);

  // vertices for a RightTriangle
  static const GLfloat constexpr vertices[] = {
      -1.0f, -1.0f, 0.0f, // x,y,z vertex 1
      1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
      1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
  };

  // move data into graphics memory
  void buffer();
};

// init function
Triangle::Triangle(GLuint shader_program_id, GLuint init_vbo)
{
  draw_length = (sizeof(vertices) / sizeof(*vertices));
  shader = shader_program_id;
  vbo = init_vbo;

  mem_type = GL_STATIC_DRAW;

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
}

void Triangle::buffer()
{
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, mem_type);
}

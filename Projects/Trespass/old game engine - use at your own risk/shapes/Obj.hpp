
class Obj: public ShapeBase
{
private:
    /* data */
public:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals; // Won't be used at the moment.

  Obj(GLuint shader_program_id, GLuint init_memory_type);
  ~Obj();
  void draw();
  void load(std::string file);
};

Obj::Obj(GLuint shader_program_id, GLuint init_memory_type)
{
    draw_type = GL_STATIC_DRAW;
  

    shader = shader_program_id;
    mem_type = init_memory_type;

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
}

// void Obj::load(std::string file){
//       // Read our .obj file
//       const char* file_name = file.c_str();
//       bool res = loadOBJ(file_name, vertices, uvs, normals);
// }

void Obj::draw()
{

    glUseProgram(shader);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glVertexAttribPointer(
    //     0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
    //     3,        // size
    //     GL_FLOAT, // type
    //     GL_FALSE, // normalized?
    //     0,        // stride
    //     (void *)0 // array buffer offset
    // );
    // // Draw the RightTriangle !
    glColor3b(20, 20, 10);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], draw_type);
    glDisableVertexAttribArray(0);
}


Obj::~Obj()
{
}

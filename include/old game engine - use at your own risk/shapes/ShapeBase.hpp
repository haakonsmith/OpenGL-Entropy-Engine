
#ifndef xy
#define xy

enum xy 
{
  x,
  y,
  z
};

#endif

class ShapeBase
{

private:


  int coordinateSpace = 20;

  float coordtofloat(float coord, int dire)
  {

    switch (dire)
    {
      case x:
        coord = coord - (SCREEN_WIDTH / 2);
        break;
    
      case y:
        coord = coord - ((SCREEN_HEIGHT) / 2);
        break;
      case z:

      break;
    }
    
    return coord / coordinateSpace;
  }

  int floattoint(float inte)
  {
    return inte * coordinateSpace;
  }



public:
  unsigned int draw_length;

  glm::vec3 model_trans = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 model_scl = glm::vec3(1.0f, 1.0f, 1.0f);
  int model_rot = 1;

  std::vector<glm::vec3> vertices;

  // this is our shader
  GLuint shader;

  // This will identify our vertex buffer
  GLuint mem_type;



  // graphics card memory type
  GLuint vbo;

  // is the model transformed?
  bool transformed = false;

  GLuint MatrixID;

  glm::mat4 MVP;

  GLuint draw_type;

  GLuint VertexArrayID;

  // draw shape
  void draw();

  // move data into graphics memory
  void buffer();

  ~ShapeBase();

  // transform
  void transform(float rotation, glm::vec3 translation, glm::vec3 scale, float rot);
};

void ShapeBase::transform(float rotation = 0, glm::vec3 translation = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float rot = 45.0)
{


  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(shader, "MVP");

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::degrees(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // Or, for an ortho camera :
  // glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates

  // glm::mat4 Projection = glm::ortho(-10.0f,// left
  //                                   10.0f,  // right
  //                                   -10.0f, // bottom
  //                                   10.0f,             // top
  //                                   0.0f,             // zNear
  //                                   100.0f              // zFar
  // );

  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(0, 0, 6),           // Camera is at (2,3,3), in World Space
      glm::vec3(0, 0, 0),         // and looks at the origin
      glm::vec3(0, 1, 0)                // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 model = glm::mat4(1.0f);

  glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);

  glm::mat4 model_rotation = glm::rotate(
      model,
      (model_rot + rotation - 1) * glm::radians(1.0f), // minus 1 for rotiaon correction
      glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 model_translation = glm::translate(model, glm::vec3(coordtofloat(translation[0], x), coordtofloat(translation[1], y), coordtofloat(translation[2], z)) * model_trans * glm::vec3(1.0f, 1.0f, 1.0f));
  glm::mat4 model_scale = glm::scale(model, scale * model_scl * glm::vec3(1.0f, -1.0f, 1.0f));
  model = model_translation * model_rotation * model_scale;

  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View * model; // Remember, matrix multiplication is the other way around

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));

  return;
}

void ShapeBase::draw()
{

  glUseProgram(shader);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(
      0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );
  // Draw the RightTriangle !
  glColor3i(2,1,1);
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 RightTriangle
  glDisableVertexAttribArray(0);
}



ShapeBase::~ShapeBase(){
  // glDeleteBuffers(1, &vertexbuffer);
  // glDeleteProgram(shader);
  // glDeleteVertexArrays(1, &VertexArrayID);
}

void ShapeBase::buffer(){
  // // Generate 1 buffer, put the resulting identifier in vertexbuffer
  // glGenBuffers(1, &vbo);

  // // The following commands will talk about our 'vertexbuffer' buffer
  // glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // // Give our vertices to OpenGL.
  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], mem_type);
}
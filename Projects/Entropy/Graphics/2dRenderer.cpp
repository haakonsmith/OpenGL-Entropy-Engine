#include "2dRenderer.hpp"

namespace Entropy
{
void m_2dRenderer::add_renderable(Renderable *_renderable)
{
  // renderables don't come prepackaged with MVP, so it needs to be created
  transform(_renderable);

  // Create a buffer object for our Renderable
  glGenBuffers(1, &_renderable->bufferobject);

  GL_LOG("add renderable, gen buffer " << _renderable->bufferobject);

  glBindBuffer(GL_ARRAY_BUFFER, _renderable->bufferobject);

  // std::vector<GLfloat> vertices = {
  //     -1.0f, -1.0f, 0.0f, // x,y,z vertex 1
  //     1.0f, -1.0f, 0.0f,  // x,y,z vertex 2
  //     1.0f, 1.0f, 0.0f,   // x,y,z vertex 3
  // };

  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBufferData(
   GL_ARRAY_BUFFER,
   _renderable->vertices.size() * sizeof(GLfloat),
   &_renderable->vertices.front(),
   GL_STATIC_DRAW
);

  GL_LOG("add buffer data ");

  objects.push_back(_renderable);
}

void m_2dRenderer::renderFrame()
{
  for (size_t i = 0; i < objects.size(); i++)
  {
    render(objects[i]);
  }
}

void m_2dRenderer::transform(Renderable *obj)
{
  glm::mat4 MVP;

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  // glm::mat4 Projection = glm::perspective(glm::degrees(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // // Or, for an ortho camera :
  glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates

  // glm::mat4 Projection = glm::ortho(-10.0f,// left
  //                                   10.0f,  // right
  //                                   -10.0f, // bottom
  //                                   10.0f,             // top
  //                                   0.0f,             // zNear
  //                                   100.0f              // zFar
  // );

  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(0, 0, 6), // Camera is at (2,3,3), in World Space
      glm::vec3(0, 0, 0), // and looks at the origin
      glm::vec3(0, -1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 model_rotation = glm::rotate(
      model,
      glm::radians((float) obj->rotation),
      glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 model_translation = glm::translate(model, coordinate_transform(obj->position));

  glm::mat4 model_scale = glm::scale(model, obj->scale);
  model = model_translation * model_rotation * model_scale;

  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View * model; // Remember, matrix multiplication is the other way around

  obj->MVP = MVP;
}

void m_2dRenderer::render(Renderable *obj)
{
  // glBindBuffer(GL_ARRAY_BUFFER, obj.bufferobject);
  glUseProgram(programID);
  GL_LOG("bind shader ");

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(obj->MVP));

  GL_LOG("bind uniform ");
  // LOG(glm::to_string(obj->MVP));


  
  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, obj->bufferobject);
  GL_LOG("bind buffer " << obj->bufferobject);

  glVertexAttribPointer(
      0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );
  GL_LOG("Atrib pointer");
  // Draw the RightTriangle !

  // gl colour fail?
  // glColor3i(2, 1, 1);
  // GL_LOG("assign colour");

  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 RightTriangle
  GL_LOG("draw arrays ");
  glDisableVertexAttribArray(0);

  GL_LOG("Render");
}

m_2dRenderer::m_2dRenderer(unsigned int width, unsigned int height)
{
  SCREEN_HEIGHT = height;
  SCREEN_WIDTH = width;

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GL_LOG("bind vertex array");

  std::cout << "starting renderer init" << std::endl;

  programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

  GL_LOG("Renderer init");
}

m_2dRenderer::~m_2dRenderer()
{

  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  for (auto v : objects)
    delete v;

  objects.clear();

  std::cout << "Cleaning up renderer" << std::endl;
}

float m_2dRenderer::coordinate_transform(float coord, int dire) {

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

glm::vec3 m_2dRenderer::coordinate_transform(glm::vec3 coords) {

     
    coords.x = coords.x - SCREEN_WIDTH / 2;
    coords.y = coords.y - SCREEN_HEIGHT / 2;
    
    return coords / (float) coordinateSpace;
  }

} // namespace Entropy

#include "2dRenderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Entropy
{
void m_2dRenderer::addRenderable(Renderable *_renderable)
{
  // renderables don't come prepackaged with MVP, so it needs to be created
  buffer(_renderable);

  objects.push_back(_renderable);
}

void m_2dRenderer::renderFrame()
{
  for (auto obj : objects)
  {
    render(obj);
  }

  for (auto obj : objects)
  {

    if (obj->isLight)
    {
      for (auto lightObj : objects)
      {

        for (auto i = 0; i < lightObj->vertices.size(); i += 3)
        {

          // {
          // LOG(worldSpace(vec3(obj->getModelMatrix() * vec4(obj->vertices[i], obj->vertices[i + 1], obj->vertices[i + 2], 1.0f))).x);
          LOG(i);
          // renderer.renderLine(vec3(glm::vec4(glm::vec3(obj->vertices[i],obj->vertices[i+1],obj->vertices[i+2]), 1.0) * obj->MVP), obj->getPosition());
          // renderLine(
          //     worldSpace(vec3(lightObj->getModelMatrix() * vec4(lightObj->vertices[i], lightObj->vertices[i + 1], lightObj->vertices[i + 2], 1.0f))),
          //     obj->getPosition());
          // renderLine
        }
      }
    }
  }

  glUseProgram(programID);

  if (debugOutline)
  {
    for (auto obj : objects)
      renderOutline(obj);
  }

  // glUseProgram(programID);

  // if (debugCenter) {
  //   for (size_t i = 0; i < objects.size(); i++)
  //     renderCenter(objects[i]);
  // }
}

void m_2dRenderer::transform(Renderable *_renderable)
{
  glm::mat4 MVP;

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <. 100 units
  // glm::mat4 Projection = glm::perspective(glm::degrees(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // // Or, for an ortho camera :
  // glm::mat4 Projection = glm::ortho((float)-(SCREEN_WIDTH/20)/2, (float)(SCREEN_WIDTH/20)/2, (float)-(SCREEN_HEIGHT/20)/2, (float)(SCREEN_HEIGHT/20)/2, 0.0f, 100.0f); // In world coordinates


  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 model = glm::mat4(1.0f);

  _renderable->rotationMatrix = glm::rotate(
      model,
      glm::radians((float)_renderable->rotation),
      glm::vec3(0.0f, 0.0f, 1.0f));

  _renderable->translationMatrix = glm::translate(model, (_renderable->getPosition()));

  _renderable->scaleMatrix = glm::scale(model, _renderable->scale);

  _renderable->setModelMatrix(_renderable->translationMatrix * _renderable->rotationMatrix * _renderable->scaleMatrix);

  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = projectionMatrix * viewMatrix * _renderable->modelMatrix; // Remember, matrix multiplication is the other way around

  _renderable->setMVP(MVP);
}

void m_2dRenderer::genVertexBuffer(Renderable *_renderable)
{
  // Create a buffer object for our Renderable
  glGenBuffers(1, &_renderable->vertexBufferID);

  GL_LOG("add renderable, gen buffer " << _renderable->vertexBufferID);

  glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);

  glBufferData(
      GL_ARRAY_BUFFER,
      _renderable->vertices.size() * sizeof(GLfloat),
      &_renderable->vertices.front(),
      GL_STATIC_DRAW);

  GL_LOG("add buffer data ");

  _renderable->vertexBufferID = (true);
}

void m_2dRenderer::genUVBuffer(Renderable *_renderable)
{
  glGenBuffers(1, &_renderable->UVBufferID);

  glBindBuffer(GL_ARRAY_BUFFER, _renderable->UVBufferID);

  glBufferData(GL_ARRAY_BUFFER, _renderable->UVs.size() * sizeof(GLfloat), &_renderable->UVs.front(), GL_STATIC_DRAW);

  GL_LOG("add buffer data ");

  _renderable->UVBufferINIT = (true);
}

void m_2dRenderer::buffer(Renderable *_renderable)
{
  // renderables don't come prepackaged with MVP, so it needs to be created
  transform(_renderable);

  if (!_renderable->vertexBufferINIT)
    genVertexBuffer(_renderable);

  if (!_renderable->UVBufferINIT)
    genUVBuffer(_renderable);

  if (!_renderable->TextureINIT)
    _renderable->texture = loadTexture("img_test.png");

  GL_LOG("finish buffer ");
}

GLuint m_2dRenderer::loadTexture(std::string path)
{

  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *image = stbi_load(path.c_str(),
                                   &width,
                                   &height,
                                   &channels,
                                   STBI_rgb);

  if (image == nullptr) //Error check
  {
    cerr << "Error when loading texture from file: " + path << endl;
  }

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  GL_LOG("finish buffer ");

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  GL_LOG("finish buffer ");

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

  GL_LOG("finish buffer ");

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  GL_LOG("finish buffer ");

  // Return the ID of the texture we just created
  return textureID;
}

void m_2dRenderer::render(Renderable *_renderable)
{
  // glBindBuffer(GL_ARRAY_BUFFER, obj.bufferobject);
  glUseProgram(programID);
  GL_LOG("bind shader ");

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(_renderable->MVP));

  GL_LOG("bind uniform ");
  // LOG(glm::to_string(obj.MVP));

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _renderable->texture);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(TextureID, 0);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);
  GL_LOG("bind buffer " << _renderable->vertexBufferID);

  glVertexAttribPointer(
      0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  GL_LOG("Atrib pointer");
  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, _renderable->UVBufferID);
  glVertexAttribPointer(
      1,        // attribute. No particular reason for 1, but must match the layout in the shader.
      2,        // size : U+V => 2
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, _renderable->vertices.size() / 3); // Starting from vertex 0; 3 vertices total . 1 RightTriangle

  GL_LOG("draw arrays ");

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  GL_LOG("Render");
}

void m_2dRenderer::renderLine(vec3 p1, vec3 p2)
{

  p1 = modelSpace(p1);
  p2 = modelSpace(p2);

  GLfloat verts[] = {p1.x, p1.y, p1.z, p2.x, p2.y, p2.z};

  // vector<GLfloat> verts = {-1,1,0,-0.3,1,0};

  GLuint VBO;

  glGenBuffers(1, &VBO);

  GL_LOG("add renderable, gen buffer " << VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(verts),
      verts,
      GL_STATIC_DRAW);

  GL_LOG("add buffer data ");
  // LOG(glm::to_string(obj.MVP));

  glUseProgram(debugLineShaderID);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  GL_LOG("bind buffer " << VBO);

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

  GL_LOG("draw arrays ");
  glDrawArrays(GL_LINES, 0, 2); // Starting from vertex 0; 3 vertices total . 1 RightTriangle
  glDisableVertexAttribArray(0);

  GL_LOG("Render");

  glDeleteBuffers(1, &VBO);
}

void m_2dRenderer::renderOutline(Renderable *_renderable)
{

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(debugShaderID, "MVP");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(_renderable->MVP));

  GL_LOG("bind uniform ");
  // LOG(glm::to_string(obj.MVP));

  glUseProgram(debugShaderID);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);
  GL_LOG("bind buffer " << _renderable->vertexBufferID);

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

  glDrawArrays(GL_LINE_LOOP, 0, _renderable->vertices.size() / 3); // Starting from vertex 0; 3 vertices total . 1 RightTriangle
  GL_LOG("draw arrays ");
  glDisableVertexAttribArray(0);

  GL_LOG("Render");
}

void m_2dRenderer::renderCenter(Renderable *_renderable)
{

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint centerID = glGetUniformLocation(programID, "MVP");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(_renderable->MVP));

  glUniform3f(centerID, 0.5, 0.5, 0.5);

  GL_LOG("bind uniform ");
  // LOG(glm::to_string(obj.MVP));

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);
  GL_LOG("bind buffer " << _renderable->vertexBufferID);

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

  glDrawArrays(GL_POINT, 0, 3); // Starting from vertex 0; 3 vertices total . 1 RightTriangle
  GL_LOG("draw arrays ");
  glDisableVertexAttribArray(0);

  GL_LOG("Render");
}

m_2dRenderer::m_2dRenderer(unsigned int width, unsigned int height)
{
  SCREEN_HEIGHT = height;
  SCREEN_WIDTH = width;

  projectionMatrix = glm::ortho(0.0f, // left
                                             (float) SCREEN_WIDTH,  // right
                                             0.0f, // bottom
                                             (float) SCREEN_HEIGHT,  // top
                                             0.0f,  // zNear
                                             100.0f // zFar
  );

  // Camera matrix
  viewMatrix = glm::lookAt(
      glm::vec3(0, 0, 6), // Camera is at (2,3,3), in World Space
      glm::vec3(0, 0, 0), // and looks at the origin
      glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );




  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GL_LOG("bind vertex array");

  std::cout << "starting renderer init" << std::endl;

  programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

  debugCenterShader = LoadShaders("shaders/centerDebug.vertexshader", "shaders/centerDebug.fragmentshader");

  debugShaderID = LoadShaders("shaders/debug/red.vertexshader", "shaders/debug/red.fragmentshader");
  debugLineShaderID = LoadShaders("shaders/debug/line.vertexshader", "shaders/debug/line.fragmentshader");

  GL_LOG("Renderer init");

  GLfloat lineWidthRange[2] = {0.0f, 0.0f};
  glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
  LOG(lineWidthRange[1]);

  LOG("!!! HARDWARE INFO !!!");
  LOG("Min line width: " << lineWidthRange[0]);
  LOG("Max line width: " << lineWidthRange[1]);
}

m_2dRenderer::~m_2dRenderer()
{

  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // for (auto v : objects)
  //   if (!dynamic_cast<Renderable>(v))
  //     delete v;

  objects.clear();

  std::cout << "Cleaning up renderer" << std::endl;
}

glm::vec3 m_2dRenderer::modelSpace(glm::vec3 worldSpaceCoords)
{

  // coords.x = (coords.x - (SCREEN_WIDTH / 2))/SCREEN_WIDTH * 2;
  worldSpaceCoords.x = ((worldSpaceCoords.x * 2) / SCREEN_WIDTH) - 1;
  worldSpaceCoords.y = ((worldSpaceCoords.y * 2) / SCREEN_HEIGHT) - 1;

  return worldSpaceCoords;
}

glm::vec3 m_2dRenderer::worldSpace(glm::vec3 modelSpaceCoords)
{

  // coords.x = (coords.x - (SCREEN_WIDTH / 2))/SCREEN_WIDTH * 2;
  modelSpaceCoords.x = (modelSpaceCoords.x * SCREEN_WIDTH + SCREEN_WIDTH) / 2;
  modelSpaceCoords.y = (modelSpaceCoords.y * SCREEN_HEIGHT + SCREEN_HEIGHT) / 2;

  return modelSpaceCoords;
}

} // namespace Entropy

#include "2dRenderer.hpp"

namespace Entropy
{
void m_2dRenderer::add_renderable(Renderable* _renderable)
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

  glUseProgram(programID);

  if (debugOutline) {
    for (auto obj : objects) 
      renderOutline(obj);
  }

  // glUseProgram(programID);

  // if (debugCenter) {
  //   for (size_t i = 0; i < objects.size(); i++) 
  //     renderCenter(objects[i]);
  // }
}

void m_2dRenderer::transform(Renderable* obj)
{
  glm::mat4 MVP;

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <. 100 units
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
      glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 model_rotation = glm::rotate(
      model,
      glm::radians((float) obj->rotation),
      glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 model_translation = glm::translate(model, coordinate_transform(obj->getPosition()));


  glm::mat4 model_scale = glm::scale(model, obj->scale);
  model = model_translation * model_rotation * model_scale;

  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP = Projection * View * model; // Remember, matrix multiplication is the other way around

  obj->MVP = MVP;
}

void m_2dRenderer::genVertexBuffer(Renderable* _renderable)
{
  // Create a buffer object for our Renderable
  glGenBuffers(1, &_renderable->vertexBufferID);

  GL_LOG("add renderable, gen buffer " << _renderable->vertexBufferID);

  glBindBuffer(GL_ARRAY_BUFFER, _renderable->vertexBufferID);

  glBufferData(
      GL_ARRAY_BUFFER,
      _renderable->vertices.size() * sizeof(GLfloat),
      &_renderable->vertices.front(),
      GL_STATIC_DRAW
    );

  GL_LOG("add buffer data ");
}

void m_2dRenderer::genUVBuffer(Renderable* _renderable)
{
	glGenBuffers(1, &_renderable->UVBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, _renderable->UVBufferID);
  
	glBufferData(GL_ARRAY_BUFFER, _renderable->UVs.size() * sizeof(GLfloat), &_renderable->UVs.front(), GL_STATIC_DRAW);

  GL_LOG("add buffer data ");
}

void m_2dRenderer::buffer(Renderable* _renderable)
{
  // renderables don't come prepackaged with MVP, so it needs to be created
  transform(_renderable);

  genVertexBuffer(_renderable);  

  genUVBuffer(_renderable);

  _renderable->texture = loadTexture("img_test.png");

  GL_LOG("finish buffer ");
}


GLuint m_2dRenderer::loadTexture(std::string path) {
  
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *image = stbi_load(path.c_str(),
                                  &width,
                                  &height,
                                  &channels,
                                  STBI_rgb);

  if (image == nullptr)//Error check
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

void m_2dRenderer::render(Renderable* obj)
{
  // glBindBuffer(GL_ARRAY_BUFFER, obj.bufferobject);
  glUseProgram(programID);
  GL_LOG("bind shader ");

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

  GL_LOG("get uniform ");



  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(obj->MVP));

  GL_LOG("bind uniform ");
  // LOG(glm::to_string(obj.MVP));

  // Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);


  
  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBufferID);
  GL_LOG("bind buffer " << obj->vertexBufferID);

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
  glBindBuffer(GL_ARRAY_BUFFER, obj->UVBufferID);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size : U+V => 2
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total . 1 RightTriangle

  GL_LOG("draw arrays ");

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);


  GL_LOG("Render");
}

void m_2dRenderer::renderOutline(Renderable* _renderable)
{
 
  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(_renderable->MVP));

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

  glDrawArrays(GL_LINE_LOOP, 0, 3); // Starting from vertex 0; 3 vertices total . 1 RightTriangle
  GL_LOG("draw arrays ");
  glDisableVertexAttribArray(0);

  GL_LOG("Render");
}

void m_2dRenderer::renderCenter(Renderable* _renderable)
{
 
  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint centerID = glGetUniformLocation(programID, "MVP");

  GL_LOG("get uniform ");

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(_renderable->MVP));

  glUniform3f(centerID, 0.5,0.5,0.5);


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

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GL_LOG("bind vertex array");

  std::cout << "starting renderer init" << std::endl;

  programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

  debugCenterShader = LoadShaders("shaders/centerDebug.vertexshader", "shaders/centerDebug.fragmentshader");

  GL_LOG("Renderer init");
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

#ifndef M_2DRENDERER
#define GL_SILENCE_DEPRECATION
#define M_2DRENDERER
#include "RenderInstance.hpp"
#include "RenderTarget.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
// #define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
// #include <glm/gtx/string_cast.hpp>
#include "../Shared.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#else
#error "2d renderer included twice"
#endif

#pragma once
namespace Entropy
{
class m_2dRenderer
{
private:
  unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

  bool debugOutline = false, debugCenter = false;

  mat4 viewMatrix, projectionMatrix;

  shared_ptr<Shader> program;

  shared_ptr<Renderable> debugQuad;
  GLuint VertexArrayID;
  GLuint programID;
  GLuint debugCenterShader;
  shared_ptr<Shader> debugShader;
  shared_ptr<Shader> instanceShader;
  shared_ptr<Shader> debugLineShader;
  std::vector<Renderable *> objects = std::vector<Renderable *>();

  /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
  void renderCenter(Renderable *_renderable);

  /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
  void renderOutline(const Renderable &_renderable);

  void genVertexBuffer(Renderable *_renderable);
  void genUVBuffer(Renderable *_renderable);

public:
  template <uint32 C>
  Ref<RenderInstance<C>> getRenderInstance(Ref<Renderable> renderable) { return make_shared<RenderInstance<C>>(renderable); }

  map<string, RenderTarget> frameBuffers;

  void createRenderTarget(string name)
  {
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    frameBuffers.insert({name, RenderTarget()});

    frameBuffers[name].FrameBuffer = 0;
    glGenFramebuffers(1, &frameBuffers[name].FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[name].FrameBuffer);

    // The texture we're going to render to
    frameBuffers[name].Texture = 0;
    glGenTextures(1, &frameBuffers[name].Texture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, frameBuffers[name].Texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBuffers[name].Texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      throw "Failed to create frame buffer";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void bindRenderTarget(string name)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[name].FrameBuffer);

    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[name].FrameBuffer);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
  };

  template <uint32 C>
  void renderInstance(const RenderInstance<C> &Instance)
  {

    glBindBuffer(GL_ARRAY_BUFFER, Instance.Instanced->vertexBufferID);

    Instance.Instanced->shader->Bind();
    GL_LOG("bind shader ");

    Instance.Instanced->shader->UniformMatrix4fv("MVP", Instance.Instanced->MVP);
    GL_LOG("bind shader ");

    glActiveTexture(GL_TEXTURE0);
    GL_LOG("Active Texture");
    glBindTexture(GL_TEXTURE_2D, Instance.Instanced->texture);
    GL_LOG("Bind Texture");

    // Set our "myTextureSampler" sampler to use Texture Unit 0
    Instance.Instanced->shader->Uniform1i("myTextureSampler", 0);
    GL_LOG("Use shader");

    // 1st attribute buffer : Vertices

    GL_LOG("Atrib pointer");

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,              // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,              // count
        GL_FLOAT,       // type
        GL_FALSE,       // normalized?
        sizeof(Vertex), // stride
        (void *)0       // array buffer offset
    );
    GL_LOG("Atrib ptr");

    glEnableVertexAttribArray(1);
    GL_LOG("Atrib ptr");
    glVertexAttribPointer(
        1,              // attribute. No particular reason for 1, but must match the layout in the shader.
        2,              // size : U+V => 2
        GL_FLOAT,       // type
        GL_FALSE,       // normalized?
        sizeof(Vertex), // stride
        (void *)12      // array buffer offset
    );
    GL_LOG("Atrib ptr");

    glEnableVertexAttribArray(2);
    GL_LOG("Atrib instance");
    Instance.PositionBuffer->Bind();
    GL_LOG("Bind instance position buffer");
    glVertexAttribPointer(
        2,            // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,            // count
        GL_FLOAT,     // type
        GL_FALSE,     // normalized?
        sizeof(vec3), // stride
        (void *)0     // array buffer offset
    );

    GL_LOG("Render");
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 0); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    glDrawArraysInstanced(GL_TRIANGLES, 0, Instance.Instanced->Vertices.size(), C);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  /**
       * Converts coordinate vector coordinate to OpenGL float space
       */
  glm::vec3 modelSpace(glm::vec3 coords);

  glm::vec3 worldSpace(glm::vec3 coords);

  void removeRenderable(Renderable *renderable);
  /**
        * Add renderable object to objects.
        * 
        * Required for some renderer optimizations
        */
  void addRenderable(Renderable *_renderable);

  float distToNearestPoint(vec3 point);
  float distToNearestPoint(vec3 point, Renderable *renderable);
  float distToNearestPoint(Renderable *_renderable);

  void renderQuad(vec3 position, float width, float height, bool hollow = false, vec3 color = vec3(1, 0, 0));

  /**
        * Draw line between two world space points
        */
  void renderLine(const vec3 &p1, const vec3 &p2);

  GLuint loadTexture(std::string path);

  /**
       * Render a renderable without adding to the renderer.
       */
  void render(Renderable *obj);

  /**
       * Generate all buffers for a renderables.
       */
  void buffer(Renderable *_renderable);

  /**
       * Render all renderables in renderer.
       */
  void renderFrame();

  /**
       * Enables drawing of the center of graphics, for debugging.
       * !!!!BROKEN!!!!
       */
  void drawCenter(bool val) { debugCenter = val; };

  /**
       * Enables drawing of the outline of graphics, for debugging.
       * Different to collision outline debug.
       * !!!!BROKEN!!!!
       */
  void drawOutline(bool val) { debugOutline = val; };

  /**
       * Update renderable model view matrix
       */
  void transform(Renderable *obj);

  m_2dRenderer(unsigned int width, unsigned int height);
  ~m_2dRenderer();
};
} // namespace Entropy

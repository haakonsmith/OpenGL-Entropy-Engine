/*
 * Copyright 2020, Haakon Smith.
 */

#ifndef M_2DRENDERER
#define GL_SILENCE_DEPRECATION
#define M_2DRENDERER
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <fstream>
#include <iosfwd>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "2dRenderInstance.hpp"
#include "2dRenderInterface.hpp"
#include "RenderTarget.hpp"
#include "Renderable.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
// #define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
// #include <glm/gtx/string_cast.hpp>
#include "../Screen.hpp"
#include "../Shared.hpp"
#include "LightRendererAttachment.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#else
#error "2d renderer included twice"
#endif

#pragma once
namespace Entropy {
    class m_2dRenderer : public LightRendererAttachment {
      private:
        bool debugOutline = false, debugCenter = false;

        mat4 viewMatrix, projectionMatrix, viewProjectionMatrix;

        shared_ptr<Shader> program;

        Screen &screen;

        shared_ptr<Renderable> debugQuad;
        VertexArray vertexArray;
        GLuint programID;
        GLuint debugCenterShader;
        shared_ptr<Shader> debugShader;
        shared_ptr<Shader> instanceShader;
        shared_ptr<Shader> debugLineShader;
        shared_ptr<Shader> builtinCircleShader;
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
        Ref<RenderInstance<C>> getRenderInstance(Ref<Renderable> renderable) {
            return make_shared<RenderInstance<C>>(renderable);
        }

        inline mat4 getViewProjectionMatrix() { return viewProjectionMatrix; }

        map<string, RenderTarget> frameBuffers;

        void createRenderTarget(string name) {
            // The framebuffer, which regroups 0, 1, or more textures, and 0 or
            // 1 depth buffer.
            frameBuffers.insert({name, RenderTarget()});

            frameBuffers[name].FrameBuffer = 0;
            glGenFramebuffers(1, &frameBuffers[name].FrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[name].FrameBuffer);

            // The texture we're going to render to
            frameBuffers[name].texture = Texture(Texture::createBlank());

            // "Bind" the newly created texture : all future texture functions
            // will modify this texture
            frameBuffers[name].texture.bind();

            // Give an empty image to OpenGL ( the last NULL )
            frameBuffers[name].texture.upload(screen.sizeX, screen.sizeY, NULL);

            // Poor filtering. Needed !
            frameBuffers[name].texture.setPoorFiltering();

            // Set "renderedTexture" as our colour attachement #0
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBuffers[name].texture.getID(), 0);

            // Set the list of draw buffers.
            GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, DrawBuffers);  // "1" is the size of DrawBuffers

            // Always check that our framebuffer is ok
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Failed to create frame buffer");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void bindRenderTarget(string name) {
            // Render to our framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers.at(name).FrameBuffer);
            glViewport(0, 0, screen.sizeX, screen.sizeY);  // Render on the whole framebuffer,
                                                           // complete from the lower left corner
                                                           // to the upper right
        };

        void unbindRenderTarget() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, 640 * 2, 480 * 2);
        };

        void bindRenderTexture(string name) { frameBuffers.at(name).texture.bind(); }

        /**
         * C is size of data
         * Imperfect accuracy, possibly due to floating point precision.
         */
        template <uint32 C>
        void renderInstance(const RenderInstance<C> &Instance, uint32 renderCount = C);

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
         * !!!Not state safe!!!
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

        void renderCircle(vec3 position, float radius, bool hollow = false);

        m_2dRenderer(Screen &_s);
        ~m_2dRenderer();
    };
}  // namespace Entropy

#include "2dRenderer.tcc"

/*
 * Copyright 2020, Haakon Smith.
 */

#include "Texture.hpp"
#include "../Screen.hpp"

#pragma once
namespace Entropy {

    struct RenderTarget {
        GLuint frameBuffer;
        Texture texture;
        const Screen& screen;

        RenderTarget(const Screen& _screen) : screen(_screen) {
            // The framebuffer, which regroups 0, 1, or more textures, and 0 or
            // 1 depth buffer.
            frameBuffer = 0;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

            // The texture we're going to render to
            texture = Texture(Texture::createBlank());

            // "Bind" the newly created texture : all future texture functions
            // will modify this texture
            texture.bind();

            // Give an empty image to OpenGL ( the last NULL )
            texture.upload(screen.sizeX * 2, screen.sizeY * 2, NULL);

            // Poor filtering. Needed !
            texture.setPoorFiltering();

            // Set "renderedTexture" as our colour attachement #0
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.getID(), 0);

            // Set the list of draw buffers.
            GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, DrawBuffers);  // "1" is the size of DrawBuffers

            // Always check that our framebuffer is ok
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Failed to create frame buffer");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        static void unbind(const Screen& screen) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, screen.sizeX * 2, screen.sizeY * 2);
        }

        inline void bind() {
            // Render to our framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glViewport(0, 0, screen.sizeX * 2, screen.sizeY * 2);  // Render on the whole framebuffer,
                                                                   // complete from the lower left corner
                                                                   // to the upper right
        }
    };
}  // namespace Entropy

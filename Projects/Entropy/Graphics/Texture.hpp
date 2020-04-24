
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <iostream>
#include <string>

#pragma once

namespace Entropy {

    class Texture {
      private:
        std::string path;

        GLuint obj_ = 0;  // Cannot leave this uninitialised
        GLenum textureSlot = GL_TEXTURE0;

        inline void Release() {
            glDeleteTextures(1, &obj_);
            obj_ = 0;
        }

      public:
        static GLuint loadTexture(std::string path);
        static GLuint createBlank();
        static void setPoorFiltering();
        inline void upload(int width, int height, const void *data);
        inline GLuint getID() { return obj_; }

        Texture(std::string p);
        Texture(GLuint id) : obj_(id) {}
        Texture() {}

        inline void bind() {
            glActiveTexture(textureSlot);
            glBindTexture(GL_TEXTURE_2D, obj_);
        }
        // Free up the texture.
        ~Texture() { Release(); }

        // Delete the copy constructor/assignment.
        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        Texture(Texture &&other) : obj_(other.obj_) {
            other.obj_ = 0;  // Use the "null" texture for the old object.
        }

        Texture &operator=(Texture &&other) {
            // ALWAYS check for self-assignment.
            if (this != &other) {
                Release();
                // obj_ is now 0.
                std::swap(obj_, other.obj_);
            }
            return *this;
        }
    };

}  // namespace Entropy

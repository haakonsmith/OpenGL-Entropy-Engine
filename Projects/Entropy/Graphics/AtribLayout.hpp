#include <OpenGL/gl3.h>

#pragma once
namespace Entropy {

    struct VertexArray {
        GLuint obj_ = 0;  // Cannot leave this uninitialised

        inline void Release() {
            glDeleteVertexArrays(1, &obj_);
            obj_ = 0;
        }

      public:
        inline void bind() const { glBindVertexArray(obj_); }
        inline void unbind() const { glBindVertexArray(0); }

        VertexArray() { glGenVertexArrays(1, &obj_); }

        // Free up the texture.
        ~VertexArray() { Release(); }

        // Delete the copy constructor/assignment.
        VertexArray(const VertexArray &) = delete;
        VertexArray &operator=(const VertexArray &) = delete;

        VertexArray(VertexArray &&other) : obj_(other.obj_) {
            other.obj_ = 0;  // Use the "null" texture for the old object.
        }

        VertexArray &operator=(VertexArray &&other) {
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

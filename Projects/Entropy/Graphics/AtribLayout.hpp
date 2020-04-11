#include <OpenGL/gl3.h>

#pragma once
namespace Entropy {
    // struct AtribLayoutPartial {
    //     // The atrib location, identical to that of the shader.
    //     const unsigned int location;

    //     // Number of values in partial.
    //     const unsigned int count;

    //     // The distance to the start of the next partial in the GPU.
    //     const unsigned int stride;

    //     // Offset in the GPU buffer/array.
    //     const unsigned int offset;

    //     constexpr AtribLayoutPartial(unsigned int atribLocation, unsigned int numMembers, unsigned int dataSize,
    //                                  unsigned int membersSize)
    //         : location(atribLocation), count(numMembers), stride(dataSize), offset(membersSize) {}

    //     inline void bind() {}
    // };

    template <unsigned int location, unsigned int count, unsigned int stride, unsigned int offset>
    struct AtribLayout {
        // Requires that it is enabled.
        inline void bind() {
            glVertexAttribPointer(location,       // but must match the layout in the shader.
                                  count,          // Count. Size = 3 * 4 = 12
                                  GL_FLOAT,       // Type
                                  GL_FALSE,       // Normalized?
                                  stride,         // stride
                                  (void *)offset  // Array buffer offset
            );
        }
    };

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

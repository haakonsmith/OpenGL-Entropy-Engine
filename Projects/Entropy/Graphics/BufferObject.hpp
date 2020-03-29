#include "../Shared.hpp"

#pragma once

template <GLenum T>
struct BufferObject {
  protected:
    GLuint bufferObjectID = 0;
    uint32_t dataSize = 0;
    GLenum drawType;

    void Release() {
        glDeleteBuffers(1, &bufferObjectID);
        bufferObjectID = 0;
    }

    virtual void BufferData(uint32_t size, const void *data, GLenum drawType) {
        glBufferData(T, size, data, drawType);
        GL_LOG("create ");
    }

  public:
    BufferObject(unsigned int bo, uint32_t size, const void *data, GLenum draw_type)
        : drawType(draw_type), bufferObjectID(bo) {
        glGenBuffers(1, &bufferObjectID);
        GL_LOG("create ");
        glBindBuffer(T, bufferObjectID);
        GL_LOG("create ");
        BufferData(size, data, drawType);
        GL_LOG("create ");
        dataSize = size;
    }

    ~BufferObject() {
        Unbind();
        Release();
    }

    inline void Bind() const { glBindBuffer(T, bufferObjectID); }

    inline void Unbind() const { glBindBuffer(T, 0); }

    inline void SubBuffer(unsigned int offset, uint32_t size, const void *data) {
        Bind();
        glBufferSubData(T, offset, size, data);
    }

    // Delete the copy constructor/assignment.
    BufferObject(const BufferObject &) = delete;
    BufferObject &operator=(const BufferObject &) = delete;

    BufferObject(BufferObject &&other) : bufferObjectID(other.bufferObjectID) {
        other.bufferObjectID = 0;  // Use the "null" texture for the old object.
    }

    BufferObject &operator=(BufferObject &&other) {
        // ALWAYS check for self-assignment.
        if (this != &other) {
            Release();
            // obj_ is now 0.
            std::swap(bufferObjectID, other.bufferObjectID);
        }
    }
};
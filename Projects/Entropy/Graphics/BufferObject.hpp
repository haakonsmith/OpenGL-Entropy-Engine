#include "../Shared.hpp"

#pragma once

template <GLenum T>
struct BufferObject {
  protected:
    GLuint bufferObjectID = 0;
    uint32_t dataSize = 0;
    GLenum drawType;

    void release() {
        glDeleteBuffers(1, &bufferObjectID);
        bufferObjectID = 0;
    }

    virtual void bufferData(uint32_t size, const void *data, GLenum drawType) {
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
        bufferData(size, data, drawType);
        GL_LOG("create ");
        dataSize = size;
    }

    ~BufferObject() {
        unBind();
        release();
    }

    inline void bind() const { glBindBuffer(T, bufferObjectID); }

    inline void unBind() const { glBindBuffer(T, 0); }

    inline void subBuffer(unsigned int offset, uint32_t size, const void *data) {
        bind();
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
            release();
            // obj_ is now 0.
            std::swap(bufferObjectID, other.bufferObjectID);
        }
    }
};
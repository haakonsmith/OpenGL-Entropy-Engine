#include "../Shared.hpp"
#include "BufferObject.hpp"
#include "Vertex.hpp"
#include <vector>

#pragma once

struct VertexBuffer : public BufferObject<GL_ARRAY_BUFFER>
{
public:
  VertexBuffer(unsigned int vbo, const vector<Vertex> &vertices) : BufferObject<GL_ARRAY_BUFFER>(vbo, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW)
  {
  }

  VertexBuffer(unsigned int vbo, int size, Vertex *vertices) : BufferObject<GL_ARRAY_BUFFER>(vbo, size, vertices, GL_STATIC_DRAW)
  {
  }

  VertexBuffer(unsigned int vbo, int size, Vertex *vertices, GLenum drawType) : BufferObject<GL_ARRAY_BUFFER>(vbo, size, vertices, drawType)
  {
  }

  void BufferData(uint32_t size, const void *data, GLenum drawType) override
  {
    BufferObject<GL_ARRAY_BUFFER>::BufferData(size, data, drawType);
    LOG("VBO");
  }
};
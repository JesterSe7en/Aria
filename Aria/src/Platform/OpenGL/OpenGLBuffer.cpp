#include "ariapch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

namespace Aria {

// --------------- Vertex Buffer ---------------

OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size) {
  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenGLVertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// --------------- Index Buffer ---------------

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  glGenBuffers(1, &renderer_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
}

void OpenGLIndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
}

void OpenGLIndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Aria
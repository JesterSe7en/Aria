#include "ariapch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

namespace ARIA {

// --------------- Vertex Buffer ---------------

OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size) {
  // glCreateBuffers only available in 4.5
  // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateBuffers.xhtml
  glCreateBuffers(1, &mRendererID);
  glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
  glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &mRendererID); }

void OpenGLVertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// --------------- Index Buffer ---------------

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : mCount(count) {
  glCreateBuffers(1, &mRendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void OpenGLIndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void OpenGLIndexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace ARIA
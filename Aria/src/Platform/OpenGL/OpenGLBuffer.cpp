#include "ariapch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

namespace Aria {
OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size) {
  glCreateBuffers(1, &renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies,
               GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenGLVertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }


OpenGLIndexBuffer::OpenGLIndexBuffer(float* verticies, uint32_t size) {

  // create combines gen and bind buffer funcs
  glCreateBuffers(1, &renderer_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(float), vertifces,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {}

void OpenGLIndexBuffer::Bind() const {}

void OpenGLIndexBuffer::Unbind() const {}

}  // namespace Aria
#include "ariapch.h"
#include "OpenGlBuffer.h"
#include <glad/glad.h>

namespace aria {

// --------------- Vertex Buffer ---------------

OpenGlVertexBuffer::OpenGlVertexBuffer(float *vertices, std::uint32_t size) {
  // glCreateBuffers only available in 4.5
  // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateBuffers.xhtml
  glad_glCreateBuffers(1, &renderer_id_);
  glad_glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glad_glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGlVertexBuffer::~OpenGlVertexBuffer() { glad_glDeleteBuffers(1, &renderer_id_); }

void OpenGlVertexBuffer::Bind() const { glad_glBindBuffer(GL_ARRAY_BUFFER, renderer_id_); }

void OpenGlVertexBuffer::Unbind() const { glad_glBindBuffer(GL_ARRAY_BUFFER, 0); }

// --------------- Index Buffer ---------------

OpenGlIndexBuffer::OpenGlIndexBuffer(std::uint32_t *indices, std::uint32_t count) : count_(count) {
  glad_glCreateBuffers(1, &renderer_id_);
  glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), indices, GL_STATIC_DRAW);
}

OpenGlIndexBuffer::~OpenGlIndexBuffer() { glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_); }

void OpenGlIndexBuffer::Bind() const { glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_); }

void OpenGlIndexBuffer::Unbind() const { glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}// namespace aria

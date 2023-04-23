#include "ariapch.h"
#include <Aria/Log.h>
#include "OpenGLVertexArray.h"
#include <glad/gl.h>

namespace Aria {

// ------------------------- Vertex Array -------------------------

OpenGLVertexArray::OpenGLVertexArray() : renderer_id_(0) {
  glCreateVertexArrays(1, &renderer_id_);
}
OpenGLVertexArray::~OpenGLVertexArray() {
  glDeleteVertexArrays(1, &renderer_id_);
}
void OpenGLVertexArray::Bind() const { glBindVertexArray(renderer_id_); }
void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }
void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer& vertex_buffer) {}
void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer& index_buffer) {}

// ------------------------- Vertex Buffer Layout  -------------------------

OpenGLVertexBufferLayout::OpenGLVertexBufferLayout() : stride_(0) {}

uint32_t OpenGLVertexBufferLayout::GetSizeOfType(uint32_t type) {
  switch (type) {
    case GL_FLOAT:
    case GL_UNSIGNED_INT:
      return 4;
    case GL_UNSIGNED_BYTE:
      return 1;
    default:
      ARIA_CORE_ASSERT(false);
      return 0;
  }
}

template <typename T>
void OpenGLVertexBufferLayout::Push(uint32_t count) {
    static_assert(std::is_integral<T>), "Type must be an integral type");
}

template <>
void OpenGLVertexBufferLayout::Push<float>(uint32_t count) {
    elements_.push_back({GL_FLOAT, count, GL_FALSE});
    stride_ += GetSizeOfType(GL_FLOAT) * count;
}

template <>
void OpenGLVertexBufferLayout::Push<uint32_t>(uint32_t count) {
    elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride_ += GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template <>
void OpenGLVertexBufferLayout::Push<uint8_t>(uint32_t count) {
    elements_.push_back({GL_UNSIGNED_BYTE, count, GL_FALSE});
    stride_ += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

};  // namespace Aria

#include "ariapch.h"
#include "OpenGlVertexArray.hpp"
#include "Aria/Core/Log.hpp"
#include <glad/gl.h>

namespace aria {

static GLenum ShaderPrimitiveToOpenGlPrimitive(ShaderPrimitiveType type) {
  switch (type) {
    case aria::ShaderPrimitiveType::Float:
    case aria::ShaderPrimitiveType::Float2:
    case aria::ShaderPrimitiveType::Float3:
    case aria::ShaderPrimitiveType::Float4:
    case aria::ShaderPrimitiveType::Mat2:
    case aria::ShaderPrimitiveType::Mat3:
    case aria::ShaderPrimitiveType::Mat4:
      return GL_FLOAT;

    case aria::ShaderPrimitiveType::Int:
    case aria::ShaderPrimitiveType::Int2:
    case aria::ShaderPrimitiveType::Int3:
    case aria::ShaderPrimitiveType::Int4:
      return GL_INT;

    case aria::ShaderPrimitiveType::Bool:
      return GL_BOOL;

    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primitive type");
      return 0;
  }
}

OpenGlVertexArray::OpenGlVertexArray() : renderer_id_(0), vertex_buffer_index_(0), p_index_buffer_(nullptr) {
  glCreateVertexArrays(1, &renderer_id_);
}
OpenGlVertexArray::~OpenGlVertexArray() { glDeleteVertexArrays(1, &renderer_id_); }
void OpenGlVertexArray::Bind() const { glBindVertexArray(renderer_id_); }
void OpenGlVertexArray::Unbind() const { glBindVertexArray(0); }
void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) {
  auto layout = vertex_buffer->GetLayout();
  ARIA_CORE_ASSERT(layout.GetElements().size(), "Vertex buffer has no layout defined");

  glBindVertexArray(renderer_id_);
  vertex_buffer->Bind();

  for (const auto &kElement : layout) {
    switch (kElement.shader_primitive_type_) {
      case ShaderPrimitiveType::Float:
      case ShaderPrimitiveType::Float2:
      case ShaderPrimitiveType::Float3:
      case ShaderPrimitiveType::Float4:
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribPointer(vertex_buffer_index_, kElement.GetElementCount(),
                              ShaderPrimitiveToOpenGlPrimitive(kElement.shader_primitive_type_), kElement.normalized_,
                              layout.GetStride(), (const void *) kElement.offset_);
        vertex_buffer_index_++;
        break;

      case ShaderPrimitiveType::Int:
      case ShaderPrimitiveType::Int2:
      case ShaderPrimitiveType::Int3:
      case ShaderPrimitiveType::Int4:
      case ShaderPrimitiveType::Bool:
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribIPointer(vertex_buffer_index_, kElement.GetElementCount(),
                               ShaderPrimitiveToOpenGlPrimitive(kElement.shader_primitive_type_), layout.GetStride(),
                               (const void *) kElement.offset_);
        break;

      case ShaderPrimitiveType::Mat2:
      case ShaderPrimitiveType::Mat3:
      case ShaderPrimitiveType::Mat4:
        ARIA_CORE_ASSERT(false, "TODO: implement for matrix");
        break;

      default:
        ARIA_CORE_ASSERT(false, "Unknown shader primitive type");
        break;
    }
  }

  vertex_buffers_.push_back(vertex_buffer);
}
void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) {
  glBindVertexArray(renderer_id_);
  index_buffer->Bind();
  p_index_buffer_ = index_buffer;
}

};// namespace aria

#include "ariapch.h"
#include <Aria/Log.h>
#include "OpenGLVertexArray.h"
#include <glad/gl.h>

namespace ARIA {

  static GLenum ShaderPrimitiveToOpenGLPrimative(ShaderPrimitiveType type) {
    switch (type) {
    case ARIA::ShaderPrimitiveType::Float:
    case ARIA::ShaderPrimitiveType::Float2:
    case ARIA::ShaderPrimitiveType::Float3:
    case ARIA::ShaderPrimitiveType::Float4:
    case ARIA::ShaderPrimitiveType::Mat2:
    case ARIA::ShaderPrimitiveType::Mat3:
    case ARIA::ShaderPrimitiveType::Mat4:
      return GL_FLOAT;

    case ARIA::ShaderPrimitiveType::Int:
    case ARIA::ShaderPrimitiveType::Int2:
    case ARIA::ShaderPrimitiveType::Int3:
    case ARIA::ShaderPrimitiveType::Int4:
      return GL_INT;

    case ARIA::ShaderPrimitiveType::Bool:
      return GL_BOOL;

    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primitive type");
      return 0;
    }
  }

  OpenGLVertexArray::OpenGLVertexArray()
    : mRendererID(0), mVertexBufferIndex(0), mIndexBuffer(nullptr) {
    glCreateVertexArrays(1, &mRendererID);
  }
  OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &mRendererID);
  }
  void OpenGLVertexArray::bind() const { glBindVertexArray(mRendererID); }
  void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }
  void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) {
    ARIA_CORE_ASSERT(vertex_buffer.get_layout().get_elements().size(),
      "Vertex buffer has no layout defined");

    glBindVertexArray(mRendererID);
    vertex_buffer->bind();

    const auto& layout = vertex_buffer->get_layout();
    for (const auto& element : layout) {
      switch (element.mType) {
      case ShaderPrimitiveType::Float:
      case ShaderPrimitiveType::Float2:
      case ShaderPrimitiveType::Float3:
      case ShaderPrimitiveType::Float4:
        glEnableVertexAttribArray(mVertexBufferIndex);
        glVertexAttribPointer(mVertexBufferIndex, element.get_element_count(),
          ShaderPrimitiveToOpenGLPrimative(element.mType),
          element.mNormalized, layout.get_stride(),
          (const void*)element.mOffset);
        mVertexBufferIndex++;
        break;

      case ShaderPrimitiveType::Int:
      case ShaderPrimitiveType::Int2:
      case ShaderPrimitiveType::Int3:
      case ShaderPrimitiveType::Int4:
      case ShaderPrimitiveType::Bool:
        glEnableVertexAttribArray(mVertexBufferIndex);
        glVertexAttribIPointer(mVertexBufferIndex, element.get_element_count(),
          ShaderPrimitiveToOpenGLPrimative(element.mType),
          layout.get_stride(),
          (const void*)element.mOffset);
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

    mVertexBuffers.push_back(vertex_buffer);
  }
  void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer) {
    glBindVertexArray(mRendererID);
    index_buffer->bind();
    mIndexBuffer = index_buffer;
  }

};  // namespace ARIA

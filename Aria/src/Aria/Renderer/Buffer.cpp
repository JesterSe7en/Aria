#include <memory>
#include "Aria/Core/Base.h"
#include "ariapch.h"

#include "Aria/Core/Log.h"
#include "Platform/OpenGL/OpenGlBuffer.h"

#include "Buffer.h"
#include "RendererApi.h"

namespace aria {

// -------------------------- Vertex Buffer  --------------------------

Ref<VertexBuffer> VertexBuffer::Create(float* verticies, uint32_t size) {
  RendererApi::Api api = RendererApi::GetApi();
  switch (api) {
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "No renderer API selected for vertex buffer generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlVertexBuffer>(verticies, size);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN:
      ARIA_CORE_ASSERT(false, "API selected for vertex buffer generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

// -------------------------- Index Buffer --------------------------

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  RendererApi::Api api = RendererApi::GetApi();
  switch (api) {
    case RendererApi::Api::NONE:
      ARIA_CORE_ASSERT(false, "No renderer API selected for index buffer generation")
      return nullptr;
    case RendererApi::Api::OPEN_GL:
      return std::make_shared<OpenGlIndexBuffer>(indices, count);
    case RendererApi::Api::DIRECT_X:
    case RendererApi::Api::VULKAN:
      ARIA_CORE_ASSERT(false, "API selected for index buffer generation is not implemented")
      return nullptr;
    default:
      ARIA_CORE_ASSERT(false, "Unknown API")
      return nullptr;
  }
}

// -------------------------- Buffer Layout  --------------------------

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : elements_(elements) {
  CalculateOffsetAndStride();
}

void BufferLayout::CalculateOffsetAndStride() {
  size_t offset = 0;
  for (BufferElement& element : elements_) {
    element.offset_ = offset;
    offset += element.size_;
    stride_ += element.size_;
  }
}

// -------------------------- Buffer Elements  --------------------------

BufferElement::BufferElement(ShaderPrimitiveType type, const std::string& name, bool normalized)
    : name_(name), shader_primitive_type_(type), size_(get_shader_type_size(type)), normalized_(normalized) {}

/// <summary>
/// Returns the number of elements per primitive type e.g. Float3 returns 3 as
/// there are 3 floats(elements) that comprises a Float3 type.
/// </summary>
/// <returns>uint32_t - number of elements</returns>
uint32_t BufferElement::GetElementCount() const {
  switch (shader_primitive_type_) {
    case aria::ShaderPrimitiveType::Float:
      return 1;
    case aria::ShaderPrimitiveType::Float2:
      return 2;
    case aria::ShaderPrimitiveType::Float3:
      return 3;
    case aria::ShaderPrimitiveType::Float4:
      return 4;
    case aria::ShaderPrimitiveType::Mat2:
      return 2;
    case aria::ShaderPrimitiveType::Mat3:
      return 3;
    case aria::ShaderPrimitiveType::Mat4:
      return 4;
    case aria::ShaderPrimitiveType::Int:
      return 1;
    case aria::ShaderPrimitiveType::Int2:
      return 2;
    case aria::ShaderPrimitiveType::Int3:
      return 3;
    case aria::ShaderPrimitiveType::Int4:
      return 4;
    case aria::ShaderPrimitiveType::Bool:
      return 1;
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primitive type")
      return 0;
  }
}
}  // namespace ARIA
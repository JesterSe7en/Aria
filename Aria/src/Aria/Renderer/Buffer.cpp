#include "ariapch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Aria/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Aria {

// -------------------------- Vertex Buffer  --------------------------

VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size) {
  RendererAPI::API api = RendererAPI::GetAPI();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex buffer generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexBuffer(verticies, size);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "API selected for vertex buffer generation is not implemented", );
    default:
      ARIA_CORE_ASSERT("Unknown API");
  }
}

// -------------------------- Index Buffer --------------------------

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  RendererAPI::API api = RendererAPI::GetAPI();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for index buffer generation")
    case RendererAPI::API::OpenGL:
      return new OpenGLIndexBuffer(indices, count);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false, "API selected for index buffer generation is not implemented");
    default:
      ARIA_CORE_ASSERT("Unknown API");
  }
}

// -------------------------- Buffer Layout  --------------------------

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : elements_(elements), stride_(0) {
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

BufferElement::BufferElement(ShaderPrimativeTypes type, std::string& name,
                             bool normalized = false)
    : type_(type),
      name_(name),
      normalized_(normalized),
      size_(ShaderPrimativeTypeSize(type)) {}

uint32_t BufferElement::GetElementCount() const {
  switch (type_) {
    case Aria::ShaderPrimativeTypes::Float:
      break;
    case Aria::ShaderPrimativeTypes::Float2:
      break;
    case Aria::ShaderPrimativeTypes::Float3:
      break;
    case Aria::ShaderPrimativeTypes::Float4:
      break;
    case Aria::ShaderPrimativeTypes::Mat2:
      break;
    case Aria::ShaderPrimativeTypes::Mat3:
      break;
    case Aria::ShaderPrimativeTypes::Mat4:
      break;
    case Aria::ShaderPrimativeTypes::Int:
      break;
    case Aria::ShaderPrimativeTypes::Int2:
      break;
    case Aria::ShaderPrimativeTypes::Int3:
      break;
    case Aria::ShaderPrimativeTypes::Int4:
      break;
    case Aria::ShaderPrimativeTypes::Bool:
      break;
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primative type");
      break;
  }
}
}  // namespace Aria
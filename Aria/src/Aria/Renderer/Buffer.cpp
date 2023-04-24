#include "ariapch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Aria/Log.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ARIA {

// -------------------------- Vertex Buffer  --------------------------

VertexBuffer* VertexBuffer::create(float* verticies, uint32_t size) {
  RendererAPI::API api = RendererAPI::get_api();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for vertex buffer generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexBuffer(verticies, size);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false,
          "API selected for vertex buffer generation is not implemented", );
      return nullptr;
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}

// -------------------------- Index Buffer --------------------------

IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count) {
  RendererAPI::API api = RendererAPI::get_api();
  switch (api) {
    case RendererAPI::API::None:
      ARIA_CORE_ASSERT(false,
                       "No renderer API selected for index buffer generation")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLIndexBuffer(indices, count);
      break;
    case RendererAPI::API::DirectX:
    case RendererAPI::API::Vulkan:
      ARIA_CORE_ASSERT(
          false, "API selected for index buffer generation is not implemented");
      return nullptr;
    default:
      ARIA_CORE_ASSERT("Unknown API");
      return nullptr;
  }
}

// -------------------------- Buffer Layout  --------------------------

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : mElements(elements), mStride(0) {
  calculate_offset_and_stride();
}

void BufferLayout::calculate_offset_and_stride() {
  size_t offset = 0;
  for (BufferElement& element : mElements) {
    element.mOffset = offset;
    offset += element.mSize;
    mStride += element.mSize;
  }
}

// -------------------------- Buffer Elements  --------------------------

BufferElement::BufferElement(ShaderPrimitiveType type, std::string& name,
                             bool normalized = false)
    : mName(name),
      mType(type),
      mOffset(0),
      mSize(get_shader_type_size(type)),
      mNormalized(normalized) {}

  std::string mName;
ShaderPrimitiveType mType;
size_t mOffset;
uint32_t mSize;
bool mNormalized;

/// <summary>
/// Returns the number of elements per primitive type e.g. Float3 returns 3 as
/// there are 3 floats(elements) that comprises a Float3 type.
/// </summary>
/// <returns>uint32_t - number of elements</returns>
uint32_t BufferElement::get_element_count() const {
  switch (mType) {
    case ARIA::ShaderPrimitiveType::Float:
      return 1;
    case ARIA::ShaderPrimitiveType::Float2:
      return 2;
    case ARIA::ShaderPrimitiveType::Float3:
      return 3;
    case ARIA::ShaderPrimitiveType::Float4:
      return 4;
    case ARIA::ShaderPrimitiveType::Mat2:
      return 2;
    case ARIA::ShaderPrimitiveType::Mat3:
      return 3;
    case ARIA::ShaderPrimitiveType::Mat4:
      return 4;
    case ARIA::ShaderPrimitiveType::Int:
      return 1;
    case ARIA::ShaderPrimitiveType::Int2:
      return 2;
    case ARIA::ShaderPrimitiveType::Int3:
      return 3;
    case ARIA::ShaderPrimitiveType::Int4:
      return 4;
    case ARIA::ShaderPrimitiveType::Bool:
      return 1;
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primitive type");
      return 0;
  }
}
}  // namespace ARIA
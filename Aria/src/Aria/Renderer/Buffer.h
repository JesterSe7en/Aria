#pragma once

#include <Aria/Log.h>

namespace ARIA {

enum class ShaderPrimitiveType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat2,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

/// <summary>
/// Returns the number of bytes a shader primitive type has
/// </summary>
/// <param name="type">ShaderPrimitiveType type</param>
/// <returns>uint32_t - number of bytes</returns>
static uint32_t get_shader_type_size(ShaderPrimitiveType type) {
  switch (type) {
    case ARIA::ShaderPrimitiveType::Int:
    case ARIA::ShaderPrimitiveType::Float:
      return 4;
    case ARIA::ShaderPrimitiveType::Int2:
    case ARIA::ShaderPrimitiveType::Float2:
      return 4 * 2;
    case ARIA::ShaderPrimitiveType::Int3:
    case ARIA::ShaderPrimitiveType::Float3:
      return 4 * 3;
    case ARIA::ShaderPrimitiveType::Int4:
    case ARIA::ShaderPrimitiveType::Float4:
      return 4 * 4;
    case ARIA::ShaderPrimitiveType::Mat2:
      return 4 * 2 * 2;
    case ARIA::ShaderPrimitiveType::Mat3:
      return 4 * 3 * 3;
    case ARIA::ShaderPrimitiveType::Mat4:
      return 4 * 4 * 4;
      break;
    case ARIA::ShaderPrimitiveType::Bool:
      return 1;
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader primitive type");
      return 0;
  }
}

class BufferElement {
 public:
  std::string mName;
  ShaderPrimitiveType mType;
  size_t mOffset;
  uint32_t mSize;
  bool mNormalized;

  BufferElement() = default;

  // offset and stride is calculated after it is added to the buffer layout
  BufferElement(ShaderPrimitiveType type, const std::string& name, bool normalized = false);

  uint32_t get_element_count() const;
};

class BufferLayout {
 public:
  BufferLayout() = default;
  BufferLayout(std::initializer_list<BufferElement> elements);

  std::vector<BufferElement> get_elements() { return mElements; }
  int get_stride() const { return mStride; }

  // iterators
  inline std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
  inline std::vector<BufferElement>::iterator end() { return mElements.end(); }
  inline std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
  inline std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

 private:
  int mStride;
  std::vector<BufferElement> mElements;
  void calculate_offset_and_stride();
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual const BufferLayout& get_layout() const = 0;
  virtual void set_layout(const BufferLayout& layout) = 0;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  static VertexBuffer* create(float* vertices, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual uint32_t get_count() const = 0;

  static IndexBuffer* create(uint32_t* indices, uint32_t count);
};

}  // namespace ARIA

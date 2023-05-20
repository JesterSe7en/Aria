#pragma once

#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"

namespace aria {

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
    case aria::ShaderPrimitiveType::Int:
    case aria::ShaderPrimitiveType::Float:return 4;
    case aria::ShaderPrimitiveType::Int2:
    case aria::ShaderPrimitiveType::Float2:return 4 * 2;
    case aria::ShaderPrimitiveType::Int3:
    case aria::ShaderPrimitiveType::Float3:return 4 * 3;
    case aria::ShaderPrimitiveType::Int4:
    case aria::ShaderPrimitiveType::Float4:return 4 * 4;
    case aria::ShaderPrimitiveType::Mat2:return 4 * 2 * 2;
    case aria::ShaderPrimitiveType::Mat3:return 4 * 3 * 3;
    case aria::ShaderPrimitiveType::Mat4:return 4 * 4 * 4;
    case aria::ShaderPrimitiveType::Bool:return 1;
    default:ARIA_CORE_ASSERT(false, "Unknown shader primitive type")
      return 0;
  }
}

class BufferElement {
 public:
  std::string name_;
  ShaderPrimitiveType shader_primitive_type_;
  size_t offset_ = 0;
  uint32_t size_;
  bool normalized_;

  BufferElement() = default;

  // offset and stride is calculated after it is added to the buffer layout
  BufferElement(ShaderPrimitiveType type, const std::string &name, bool normalized = false);

  uint32_t GetElementCount() const;
};

class BufferLayout {
 public:
  BufferLayout() = default;
  BufferLayout(std::initializer_list<BufferElement> elements);

  std::vector<BufferElement> GetElements() const { return elements_; }
  int GetStride() const { return stride_; }

  // iterators
  inline std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  inline std::vector<BufferElement>::iterator end() { return elements_.end(); }
  inline std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
  inline std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

 private:
  int stride_ = 0;
  std::vector<BufferElement> elements_;
  void CalculateOffsetAndStride();
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual const BufferLayout &GetLayout() const = 0;
  virtual void SetLayout(const BufferLayout &layout) = 0;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static Ref<VertexBuffer> Create(float *vertices, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;

  static Ref <IndexBuffer> Create(uint32_t *indices, uint32_t count);
};

}  // namespace ARIA

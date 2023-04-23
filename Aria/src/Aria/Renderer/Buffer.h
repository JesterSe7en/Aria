#pragma once

#include <Aria/Log.h>

namespace Aria {

  enum class ShaderPrimativeTypes {
    None = 0,
    Float, Float2, Float3, Float4,
    Mat2, Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool
  };

  /// <summary>
  /// Returns the number of bytes a shader primative type has
  /// </summary>
  /// <param name="type">ShaderPrimativeTypes type</param>
  /// <returns>uint32_t - number of bytes</returns>
  static uint32_t ShaderPrimativeTypeSize(ShaderPrimativeTypes type) {
    switch (type) {
      case Aria::ShaderPrimativeTypes::Int:
      case Aria::ShaderPrimativeTypes::Float:
        return 4;
      case Aria::ShaderPrimativeTypes::Int2:
      case Aria::ShaderPrimativeTypes::Float2:
        return 4 * 2;
      case Aria::ShaderPrimativeTypes::Int3:
      case Aria::ShaderPrimativeTypes::Float3:
        return 4 * 3;
      case Aria::ShaderPrimativeTypes::Int4:
      case Aria::ShaderPrimativeTypes::Float4:
        return 4 * 4;
      case Aria::ShaderPrimativeTypes::Mat2:
        return 4 * 2 * 2;
      case Aria::ShaderPrimativeTypes::Mat3:
        return 4 * 3 * 3;
      case Aria::ShaderPrimativeTypes::Mat4:
        return 4 * 4 * 4;
        break;
      case Aria::ShaderPrimativeTypes::Bool:
        return 1;
      default:
        ARIA_CORE_ASSERT(false, "Unknown shader primative type");
        return 0;
    }
  }

class VertexBuffer {
 public:
  virtual ~VertexBuffer(){};

  virtual void SetLayout(const BufferLayout& layout) = 0;
  virtual BufferLayout& GetLayout() const = 0;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static VertexBuffer* Create(float* verticies, uint32_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;

  static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};

class BufferLayout {
 public:
  BufferLayout() {}
  BufferLayout(std::initializer_list<BufferElement> elements);

  inline uint32_t GetStride() { return stride_; }
 private:
  uint32_t stride_;
  std::vector<BufferElement> elements_;
  void CalculateOffsetAndStride();
};

class BufferElement {
 public:
  std::string name_;
  ShaderPrimativeTypes type_;
  size_t offset_;
  uint32_t size_;
  bool normalized_;

  BufferElement() = default;

  BufferElement(ShaderPrimativeTypes type, std::string& name,
                bool normalized = false);

  uint32_t GetElementCount() const;
};

}  // namespace Aria

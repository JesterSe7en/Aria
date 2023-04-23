#pragma once

#include "Aria/Renderer/VertexArray.h"

namespace Aria {
class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  ~OpenGLVertexArray();

  void Bind() const override;
  void Unbind() const override;

  void AddVertexBuffer(const VertexBuffer& vertex_buffer);
  void SetIndexBuffer(const IndexBuffer& index_buffer);

 private:
  uint32_t renderer_id_;
};


struct VertexBufferElement {
  uint32_t type;
  uint32_t count;
  bool normalized;
};

class OpenGLVertexBufferLayout {  
 public:
  OpenGLVertexBufferLayout();

  template <typename T>
  void Push(uint32_t count);
  
  template <>
  void Push<float>(uint32_t count);

  template <>
  void Push<uint32_t>(uint32_t count);

  template <>
  void Push<uint8_t>(uint32_t count);

 private:
  static uint32_t GetSizeOfType(uint32_t type);

  std::vector<VertexBufferElement> elements_;
  uint32_t stride_;
};
}  // namespace Aria

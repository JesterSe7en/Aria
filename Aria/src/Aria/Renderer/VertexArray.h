#pragma once

#include "Aria/Renderer/Buffer.h"

namespace aria {
class VertexArray {
 public:
  ~VertexArray() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

  static Ref<VertexArray> Create();
};
}  // namespace ARIA

#pragma once

#include "Aria/Renderer/Buffer.h"

namespace Aria {
class VertexArray {
 public:
  ~VertexArray() {}

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const VertexBuffer& vertex_buffer) = 0;
  virtual void SetIndexBuffer(const IndexBuffer& index_buffer) = 0;

  static VertexArray* Create();
};
}  // namespace Aria

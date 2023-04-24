#pragma once

#include "Aria/Renderer/Buffer.h"

namespace ARIA {
class VertexArray {
 public:
  ~VertexArray() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(const VertexBuffer& vertex_buffer) = 0;
  virtual void set_index_buffer(const IndexBuffer& index_buffer) = 0;

  static VertexArray* create();
};
}  // namespace ARIA

#pragma once

#include "Aria/Renderer/Buffer.h"
#include <memory>

namespace ARIA {
class VertexArray {
 public:
  ~VertexArray() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
  virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer) = 0;

  static VertexArray* create();
};
}  // namespace ARIA

#pragma once

#include "Aria/Renderer/VertexArray.h"
#include "Aria/Renderer/Buffer.h"

namespace ARIA {
class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  ~OpenGLVertexArray();

  void bind() const override;
  void unbind() const override;

  virtual void add_vertex_buffer(const VertexBuffer& vertex_buffer) override;
  virtual void set_index_buffer(const IndexBuffer& index_buffer) override;

 private:
  uint32_t mRendererID;
  uint32_t mVertexBufferIndex;
  std::vector<const VertexBuffer*> mVertexBuffers;
  IndexBuffer* mIndexBuffer;
};
}  // namespace ARIA

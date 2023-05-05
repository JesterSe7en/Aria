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

  virtual void add_vertex_buffer(const Ref<VertexBuffer>& vertex_buffer) override;
  virtual void set_index_buffer(const Ref<IndexBuffer>& index_buffer) override;

  virtual const std::vector<Ref<VertexBuffer>>& get_vertex_buffers() const override { return mVertexBuffers; }
  virtual const Ref<IndexBuffer>& get_index_buffer() const override { return mIndexBuffer; }

 private:
  uint32_t mRendererID;
  uint32_t mVertexBufferIndex;
  std::vector<Ref<VertexBuffer>> mVertexBuffers;
  Ref<IndexBuffer> mIndexBuffer;
};
}  // namespace ARIA

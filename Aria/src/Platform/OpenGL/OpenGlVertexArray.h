#pragma once

#include "Aria/Renderer/VertexArray.h"
#include "Aria/Renderer/Buffer.h"

namespace aria {
class OpenGlVertexArray : public VertexArray {
 public:
  OpenGlVertexArray();
  ~OpenGlVertexArray();

  void Bind() const override;
  void Unbind() const override;

  void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) override;
  void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) override;

  const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const override { return vertex_buffers_; }
  const Ref<IndexBuffer> &GetIndexBuffer() const override { return p_index_buffer_; }

 private:
  uint32_t renderer_id_;
  uint32_t vertex_buffer_index_;
  std::vector<Ref<VertexBuffer>> vertex_buffers_;
  Ref<IndexBuffer> p_index_buffer_;
};
}  // namespace ARIA

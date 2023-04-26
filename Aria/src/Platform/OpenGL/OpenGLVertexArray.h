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

  virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
  virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

 private:
  uint32_t mRendererID;
  uint32_t mVertexBufferIndex;
  std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
  std::shared_ptr<IndexBuffer> mIndexBuffer;
};
}  // namespace ARIA

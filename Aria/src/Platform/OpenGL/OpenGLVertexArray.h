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

  void add_vertex_buffer(const VertexBuffer& vertex_buffer);
  void set_index_buffer(const IndexBuffer& index_buffer);

 private:
  uint32_t mRendererID;
  uint32_t mVertexBufferIndex;
  std::vector<VertexBuffer> mVertexBuffers;
  IndexBuffer& mIndexBuffer;
};
}  // namespace ARIA

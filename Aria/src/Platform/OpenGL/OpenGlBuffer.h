#pragma once

#include <cstdint>

#include "Aria/Renderer/Buffer.h"

namespace aria {

class OpenGlVertexBuffer : public VertexBuffer {
 public:
  OpenGlVertexBuffer() = default;
  OpenGlVertexBuffer(float* vertices, uint32_t size);
  ~OpenGlVertexBuffer() override;

  const BufferLayout& GetLayout() const override { return buffer_layout_; }
  void SetLayout(const BufferLayout& layout) override { buffer_layout_ = layout; }

  void Bind() const override;
  void Unbind() const override;

 private:
  BufferLayout buffer_layout_;
  uint32_t renderer_id_;
};

class OpenGlIndexBuffer : public IndexBuffer {
 public:
  OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
  ~OpenGlIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  uint32_t GetCount() const override { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};
}  // namespace ARIA
#pragma once

#include "Aria/Renderer/Buffer.h"
#include <cstdint>

namespace aria {

class OpenGlVertexBuffer : public VertexBuffer {
 public:
  OpenGlVertexBuffer() = default;
  OpenGlVertexBuffer(float *vertices, std::uint32_t size);
  ~OpenGlVertexBuffer() override;

  const BufferLayout &GetLayout() const override { return buffer_layout_; }
  void SetLayout(const BufferLayout &layout) override { buffer_layout_ = layout; }

  void Bind() const override;
  void Unbind() const override;

 private:
  BufferLayout buffer_layout_;
  std::uint32_t renderer_id_;
};

class OpenGlIndexBuffer : public IndexBuffer {
 public:
  OpenGlIndexBuffer(std::uint32_t *indices, std::uint32_t count);
  ~OpenGlIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  std::uint32_t GetCount() const override { return count_; }

 private:
  std::uint32_t renderer_id_;
  std::uint32_t count_;
};
}// namespace aria
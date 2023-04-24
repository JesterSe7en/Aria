#pragma once

#include <cstdint>

#include "Aria/Renderer/Buffer.h"

namespace ARIA {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer() = default;
  OpenGLVertexBuffer(float* verticies, uint32_t size);
  ~OpenGLVertexBuffer();

  virtual const BufferLayout& get_layout() const override {
    return mBufferLayout;
  }
  virtual void set_layout(const BufferLayout& layout) override {
    mBufferLayout = layout;
  }

  void bind() const override;
  void unbind() const override;

 private:
  BufferLayout mBufferLayout;
  uint32_t mRendererID;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
  ~OpenGLIndexBuffer();

  void bind() const override;
  void unbind() const override;

  virtual uint32_t get_count() const { return mCount; }

 private:
  uint32_t mRendererID;
  uint32_t mCount;
};
}  // namespace ARIA
#pragma once

#include <cstdint>

#include "Aria/Renderer/Buffer.h"

namespace Aria {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(float* verticies, uint32_t size);
  virtual ~OpenGLVertexBuffer();

  void Bind() const override;
  void Unbind() const override;

 private:
  uint32_t renderer_id_;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
  virtual ~OpenGLIndexBuffer();

  void Bind() const override;
  void Unbind() const override;

  virtual uint32_t GetCount() const { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};
}  // namespace Aria
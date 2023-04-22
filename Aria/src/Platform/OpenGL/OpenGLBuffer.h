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
  OpenGLIndexBuffer(float* verticies, uint32_t size);
  virtual ~OpenGLIndexBuffer();

  void Bind() const override;
  void Unbind() const override;

 private:
  uint32_t renderer_id_;
};
}  // namespace Aria
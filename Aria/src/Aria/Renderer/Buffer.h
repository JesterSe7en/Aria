#pragma once

namespace Aria {

class VertexBuffer {
 public:
  virtual ~VertexBuffer(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static VertexBuffer* Create(float* buffer, uint32_t size);
};

class IndexBuffer {
  virtual ~IndexBuffer(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static IndexBuffer* Create(float* buffer, uint32_t size);
};

}  // namespace Aria

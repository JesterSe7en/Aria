#pragma once

namespace Aria {
class IndexBuffer {
 public:
  virtual ~IndexBuffer(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static IndexBuffer* Create(float* buffer, uint32_t size);

 private:
  uint32_t renderer_id_;
};
}  // namespace Aria
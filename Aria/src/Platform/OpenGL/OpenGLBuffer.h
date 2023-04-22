#pragma once

#include "Aria/Renderer/VertexBuffer.h"
#include "Aria/Renderer/IndexBuffer.h"
#include <cstdint>

namespace Aria {
  class OpenGLBuffer : public VertexBuffer {
 public:
    virtual ~OpenGLBuffer() {}

    void Bind() const override;
    void Unbind() const override;

    static VertexBuffer* Create(float* buffer, uint32_t size);

   private:
    uint32_t renderer_id_;
  };
}
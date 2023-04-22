#include "OpenGLBuffer.h"


namespace Aria {
void Aria::OpenGLBuffer::Bind() const {}

void OpenGLBuffer::Unbind() const {}

VertexBuffer* OpenGLBuffer::Create(float* buffer, uint32_t size) {
  return nullptr;
}
}  // namespace Aria
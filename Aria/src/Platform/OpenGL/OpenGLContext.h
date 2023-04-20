#pragma once

#include "Aria/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Aria {
class OpenGLContext : public GraphicsContext {
 public:
  OpenGLContext(GLFWwindow* window_handle);

  void Init() override;
  void SwapBuffers() override;

 private:
  GLFWwindow* m_WindowHandle;
};
}  // namespace Aria

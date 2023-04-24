#pragma once

#include "Aria/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace ARIA {
class OpenGLContext : public GraphicsContext {
 public:
  OpenGLContext(GLFWwindow* window_handle);

  void init() override;
  void swap_buffers() override;

 private:
  GLFWwindow* mWindowHandle;
};
}  // namespace ARIA

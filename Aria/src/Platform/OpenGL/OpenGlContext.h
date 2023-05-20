#pragma once

#include "Aria/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace aria {
class OpenGlContext : public GraphicsContext {
 public:
  OpenGlContext(GLFWwindow *window_handle);

  void Init() override;
  void SwapBuffers() override;

 private:
  GLFWwindow *p_glfw_window;
};
}  // namespace ARIA

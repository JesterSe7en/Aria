#include "ariapch.h"
#include "OpenGLContext.h"
#include "Aria/Log.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Aria {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) {
  ARIA_CORE_ASSERT(windowHandle, "Window handle is null")
}

void OpenGLContext::Init() {
  glfwMakeContextCurrent(m_WindowHandle);

  // Load Glad
  int version = gladLoadGL(glfwGetProcAddress);
  ARIA_CORE_ASSERT(version, "Failed to initialize Glad");
  ARIA_CORE_INFO("Loaded Glad {0}.{1}", GLAD_VERSION_MAJOR(version),
                 GLAD_VERSION_MINOR(version));
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }
}  // namespace Aria
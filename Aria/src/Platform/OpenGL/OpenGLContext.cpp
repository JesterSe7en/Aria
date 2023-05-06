#include "ariapch.h"

#include "Aria/Core/Log.h"

#include "OpenGLContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace ARIA {
OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : mWindowHandle(window_handle) {
  ARIA_CORE_ASSERT(window_handle, "Window handle is null")
}

void OpenGLContext::init() {
  glfwMakeContextCurrent(mWindowHandle);

  // Load Glad
  int version = gladLoadGL(glfwGetProcAddress);
  ARIA_CORE_ASSERT(version, "Failed to initialize Glad");
  ARIA_CORE_INFO("Loaded Glad {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  ARIA_CORE_INFO("OpenGL Info:");
  // from spdlog
  //
  // Formatting of arbitrary pointers is disallowed. If you want to output
  // a pointer cast it to "void *" or "const void *". In particular, this
  // forbids formatting of "[const] volatile char *" which is printed as bool
  // by iostreams
  ARIA_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
  ARIA_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
  ARIA_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
}

void OpenGLContext::swap_buffers() { glfwSwapBuffers(mWindowHandle); }
}  // namespace ARIA
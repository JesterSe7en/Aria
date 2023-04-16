#include "ariapch.h"

#include "Aria/Core.h"
#include "Aria/Log.h"

#include "WindowsWindow.h"

namespace Aria {

static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

WindowsWindow::~WindowsWindow() {}

void WindowsWindow::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  m_Data.VSync = enabled;
}

bool WindowsWindow::IsVsync() const { return m_Data.VSync; }

void WindowsWindow::Init(const WindowProps& props) {
  m_Data.Title = props.Title;
  m_Data.Width = props.Width;
  m_Data.Height = props.Height;

  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width,
                 props.Height);

  // TODO: call glfwterminate on system shutdown
  if (!s_GLFWInitialized) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")

    s_GLFWInitialized = true;
  }

  m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
                              props.Title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);
  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);
}

void WindowsWindow::Shutdown() { glfwDestroyWindow(m_Window); }

}  // namespace Aria
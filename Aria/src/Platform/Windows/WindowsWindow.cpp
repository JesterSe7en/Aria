#include "ariapch.h"

#include "Aria/Core.h"
#include "Aria/Log.h"

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"

#include "WindowsWindow.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Aria {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error_code, const char* description) {
  ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description);
}

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
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
                              props.Title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);

  // Load Glad
  int version = gladLoadGL(glfwGetProcAddress);
  ARIA_CORE_ASSERT(version, "Failed to initialize Glad");
  ARIA_CORE_INFO("Loaded Glad {0}.{1}", GLAD_VERSION_MAJOR(version),
                 GLAD_VERSION_MINOR(version));

  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(
      m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    data.EventCallback(event);
  });

  glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        data.EventCallback(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        data.EventCallback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        data.EventCallback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(
      m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
          }
          default:
            ARIA_CORE_WARN("Unhanded action - {0}", action);
            break;
        }
      });

  glfwSetScrollCallback(
      m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.EventCallback(event);
      });

  glfwSetCursorPosCallback(
      m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xpos, (float)ypos);

        data.EventCallback(event);
      });

#pragma endregion
}

void WindowsWindow::Shutdown() { glfwDestroyWindow(m_Window); }

}  // namespace Aria
#include "ariapch.h"

#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGlContext.h"

#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace aria {

static bool s_GLFWInitialized = false;

static void GlfwErrorCallback(int error_code, const char* description){
    ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description)}

Window* Window::Create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

WindowsWindow::~WindowsWindow() {}

void WindowsWindow::OnUpdate() {
  glfwPollEvents();
  p_graphics_context_->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  window_data_.v_sync = enabled;
}

bool WindowsWindow::IsVSync() const { return window_data_.v_sync; }

void WindowsWindow::Init(const WindowProps& props) {
  window_data_.title = props.title;
  window_data_.width = props.width;
  window_data_.height = props.height;

  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height)

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")
    glfwSetErrorCallback(GlfwErrorCallback);
    s_GLFWInitialized = true;
  }

  // If not specified, will load latest from graphics driver
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  p_glfw_window_ = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);

  p_graphics_context_ = new OpenGlContext(p_glfw_window_);
  p_graphics_context_->Init();

  glfwSetWindowUserPointer(p_glfw_window_, &window_data_);
  SetVSync(true);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(p_glfw_window_, [](GLFWwindow* window, int width, int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    data.width = width;
    data.height = height;

    WindowResizeEvent event(width, height);
    data.event_callback_fn(event);
  });

  glfwSetWindowCloseCallback(p_glfw_window_, [](GLFWwindow* window) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    kData.event_callback_fn(event);
  });

  glfwSetWindowFocusCallback(p_glfw_window_, [](GLFWwindow* window, int focused) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        kData.event_callback_fn(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        kData.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(p_glfw_window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        kData.event_callback_fn(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        kData.event_callback_fn(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        kData.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(p_glfw_window_, [](GLFWwindow* window, int button, int action, int mods) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        kData.event_callback_fn(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        kData.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetScrollCallback(p_glfw_window_, [](GLFWwindow* window, double xoffset, double yoffset) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float)xoffset, (float)yoffset);
    kData.event_callback_fn(event);
  });

  glfwSetCursorPosCallback(p_glfw_window_, [](GLFWwindow* window, double xpos, double ypos) {
    WindowData const& kData = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float)xpos, (float)ypos);

    kData.event_callback_fn(event);
  });

#pragma endregion
}

void WindowsWindow::Shutdown() {
  glfwTerminate();
  glfwDestroyWindow(p_glfw_window_);
}

}  // namespace ARIA
#include "ariapch.h"

#include "Aria/Renderer/RendererApi.hpp"
#include "Platform/Windows/WindowsWindow.hpp"

#include "Aria/Core/Base.hpp"
#include "Aria/Core/Log.hpp"

#include "Aria/Events/ApplicationEvent.hpp"
#include "Aria/Events/KeyEvent.hpp"
#include "Aria/Events/MouseEvent.hpp"
#include "Platform/OpenGL/OpenGlContext.hpp"

#include <GLFW/glfw3.h>

namespace aria {

static bool s_GLFWInitialized = false;

static void GlfwErrorCallback(int error_code, const char *description) {
  ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description)
}

WindowsWindow::WindowsWindow(const WindowProps &props) {
  window_data_.title = props.title;
  window_data_.width = props.width;
  window_data_.height = props.height;
  Init();
}

WindowsWindow::~WindowsWindow() {}

void WindowsWindow::OnUpdate() {
  glfwPollEvents();
  p_context_->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  window_data_.v_sync = enabled;
}

void WindowsWindow::Init() {
  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", window_data_.title, window_data_.width, window_data_.height)

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
  CreateAriaWindow();
}

void WindowsWindow::CreateAriaWindow() {
  p_glfw_window_ = glfwCreateWindow((int) window_data_.width,
                                    (int) window_data_.height,
                                    window_data_.title.c_str(),
                                    nullptr,
                                    nullptr);
  if (RendererApi::GetApi() == RendererApi::Api::OPEN_GL) {
    p_context_ = new OpenGlContext(p_glfw_window_);
    p_context_->Init();
  }
  glfwSetWindowUserPointer(p_glfw_window_, &window_data_);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(p_glfw_window_, [](GLFWwindow *window, int width, int height) {
    WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

    data.width = width;
    data.height = height;

    WindowResizeEvent event(width, height);
    data.event_callback_fn(event);
  });

  glfwSetWindowCloseCallback(p_glfw_window_, [](GLFWwindow *window) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    data.event_callback_fn(event);
  });

  glfwSetWindowFocusCallback(p_glfw_window_, [](GLFWwindow *window, int focused) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        data.event_callback_fn(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        data.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(p_glfw_window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.event_callback_fn(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.event_callback_fn(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        data.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(p_glfw_window_, [](GLFWwindow *window, int button, int action, int mods) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        data.event_callback_fn(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        data.event_callback_fn(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetScrollCallback(p_glfw_window_, [](GLFWwindow *window, double xoffset, double yoffset) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float) xoffset, (float) yoffset);
    data.event_callback_fn(event);
  });

  glfwSetCursorPosCallback(p_glfw_window_, [](GLFWwindow *window, double xpos, double ypos) {
    WindowData const &data = *(WindowData *) glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float) xpos, (float) ypos);

    data.event_callback_fn(event);
  });

#pragma endregion
}

void WindowsWindow::Shutdown() {
  glfwTerminate();
  glfwDestroyWindow(p_glfw_window_);
}

}  // namespace ARIA

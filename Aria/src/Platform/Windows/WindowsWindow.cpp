#include "ariapch.h"

#include "Aria/Core.h"
#include "Aria/Log.h"

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace ARIA {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error_code, const char* description) {
  ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description);
}

Window* Window::create(const WindowProps& props) {
  return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) { init(props); }

WindowsWindow::~WindowsWindow() {}

void WindowsWindow::on_update() {
  glfwPollEvents();
  mContext->swap_buffers();
}

void WindowsWindow::set_vsync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  mData.mVSync = enabled;
}

bool WindowsWindow::is_vsync() const { return mData.mVSync; }

void WindowsWindow::init(const WindowProps& props) {
  mData.mTitle = props.mTitle;
  mData.mWidth = props.mWidth;
  mData.mHeight = props.mHeight;

  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", props.mTitle, props.mWidth,
                 props.mHeight);

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  // If not specified, will load latest from graphics driver
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow((int)props.mWidth, (int)props.mHeight,
                              props.mTitle.c_str(), nullptr, nullptr);

  mContext = new OpenGLContext(mWindow);
  mContext->init();
 


  glfwSetWindowUserPointer(mWindow, &mData);
  set_vsync(true);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(
      mWindow, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.mWidth = width;
        data.mHeight = height;

        WindowResizeEvent event(width, height);
        data.mEventCallback(event);
      });

  glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    data.mEventCallback(event);
  });

  glfwSetWindowFocusCallback(mWindow, [](GLFWwindow* window, int focused) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        data.mEventCallback(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        data.mEventCallback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.mEventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.mEventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        data.mEventCallback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(
      mWindow, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.mEventCallback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.mEventCallback(event);
            break;
          }
          default:
            ARIA_CORE_WARN("Unhanded action - {0}", action);
            break;
        }
      });

  glfwSetScrollCallback(
      mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.mEventCallback(event);
      });

  glfwSetCursorPosCallback(
      mWindow, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xpos, (float)ypos);

        data.mEventCallback(event);
      });

#pragma endregion
}

void WindowsWindow::shutdown() { 
  glfwTerminate();
  glfwDestroyWindow(mWindow); 
}

}  // namespace ARIA
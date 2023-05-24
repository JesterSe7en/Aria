#include "ariapch.h"
#include "VulkanWindow.hpp"
#include "Aria/Core/Base.hpp"
#include "Aria/Core/Log.hpp"
#include "Aria/Events/ApplicationEvent.hpp"
#include "Aria/Events/KeyEvent.hpp"
#include "Aria/Events/MouseEvent.hpp"

namespace aria {

static void GlfwErrorCallback(int error_code, const char *description){
    ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description)}

VulkanWindow::VulkanWindow(const WindowProps &props) {
  window_data_.title = props.title;
  window_data_.height = props.height;
  window_data_.width = props.width;
  Init();
}

void VulkanWindow::OnUpdate() { glfwPollEvents(); }

void VulkanWindow::SetVSync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  window_data_.vsync = enabled;
}

void VulkanWindow::Init() {
  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", window_data_.title, window_data_.width, window_data_.height)
  if (!glfw_initialized_) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")
    glfwSetErrorCallback(GlfwErrorCallback);
    glfw_initialized_ = !glfw_initialized_;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  CreateAriaWindow();
}

void VulkanWindow::CreateAriaWindow() {
  p_glfw_window_ = glfwCreateWindow((int) window_data_.width, (int) window_data_.height, window_data_.title.c_str(),
                                    nullptr, nullptr);
  glfwSetWindowUserPointer(p_glfw_window_, &window_data_);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(p_glfw_window_, [](GLFWwindow *window, int width, int height) {
    auto &data = *(VulkanWindowData *) glfwGetWindowUserPointer(window);

    data.width = width;
    data.height = height;

    WindowResizeEvent event(width, height);
    data.event_callback(event);
  });

  glfwSetWindowCloseCallback(p_glfw_window_, [](GLFWwindow *window) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    kData.event_callback(event);
  });

  glfwSetWindowFocusCallback(p_glfw_window_, [](GLFWwindow *window, int focused) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        kData.event_callback(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        kData.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(p_glfw_window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        kData.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        kData.event_callback(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        kData.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(p_glfw_window_, [](GLFWwindow *window, int button, int action, int mods) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        kData.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        kData.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetScrollCallback(p_glfw_window_, [](GLFWwindow *window, double xoffset, double yoffset) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float) xoffset, (float) yoffset);
    kData.event_callback(event);
  });

  glfwSetCursorPosCallback(p_glfw_window_, [](GLFWwindow *window, double xpos, double ypos) {
    auto const &kData = *(VulkanWindowData *) glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float) xpos, (float) ypos);

    kData.event_callback(event);
  });

#pragma endregion
}

}// namespace aria

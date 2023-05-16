#include "ariapch.h"

#include "VulkanWindow.h"

#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace ARIA {

static void glfw_error_callback(int error_code, const char* description){
    ARIA_CORE_ERROR("GLFW Error ({0}) - {1}", error_code, description)}

VulkanWindow::VulkanWindow(const WindowProps& props) {
  window_data.title = props.mTitle;
  window_data.height = props.mHeight;
  window_data.width = props.mWidth;
  init();
}

void VulkanWindow::on_update() { glfwPollEvents(); }

void VulkanWindow::set_vsync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  window_data.vsync = enabled;
}

void VulkanWindow::init() {
  ARIA_CORE_INFO("Creating window {0} ({1}, {2})", window_data.title, window_data.width, window_data.height)
  if (!glfw_initalized) {
    int success = glfwInit();
    ARIA_ASSERT(success, "Failed to initialize GLFW")
    glfwSetErrorCallback(glfw_error_callback);
    glfw_initalized = !glfw_initalized;
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  create_window();
}

void VulkanWindow::create_window() {
  glfw_window =
      glfwCreateWindow((int)window_data.width, (int)window_data.height, window_data.title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(glfw_window, &window_data);

#pragma region GLFW Callbacks

  glfwSetWindowSizeCallback(glfw_window, [](GLFWwindow* window, int width, int height) {
    auto& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);

    data.width = width;
    data.height = height;

    WindowResizeEvent event(width, height);
    data.event_callback(event);
  });

  glfwSetWindowCloseCallback(glfw_window, [](GLFWwindow* window) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    data.event_callback(event);
  });

  glfwSetWindowFocusCallback(glfw_window, [](GLFWwindow* window, int focused) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);

    switch (focused) {
      case GLFW_TRUE: {
        WindowFocusEvent event;
        data.event_callback(event);
        break;
      }
      case GLFW_FALSE: {
        WindowLostFocusEvent event;
        data.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", focused);
        break;
    }
  });

  glfwSetKeyCallback(glfw_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.event_callback(event);
        break;
      }
      case GLFW_REPEAT: {
        // TODO : extract actual repeat
        KeyPressedEvent event(key, 1);
        data.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow* window, int button, int action, int mods) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        data.event_callback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        data.event_callback(event);
        break;
      }
      default:
        ARIA_CORE_WARN("Unhanded action - {0}", action);
        break;
    }
  });

  glfwSetScrollCallback(glfw_window, [](GLFWwindow* window, double xoffset, double yoffset) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float)xoffset, (float)yoffset);
    data.event_callback(event);
  });

  glfwSetCursorPosCallback(glfw_window, [](GLFWwindow* window, double xpos, double ypos) {
    auto const& data = *(VulkanWindowData*)glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float)xpos, (float)ypos);

    data.event_callback(event);
  });

#pragma endregion
}

}  // namespace ARIA

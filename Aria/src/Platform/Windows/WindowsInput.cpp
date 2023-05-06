#include "ariapch.h"
#include "WindowsInput.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Input.h"

#include <GLFW/glfw3.h>

namespace ARIA {

Input* Input::sInstance = new WindowsInput();

// keycode param is a GLFW keycode for now
bool WindowsInput::is_key_pressed_impl(int keycode) {
  auto window = static_cast<GLFWwindow*>(
      Application::get().get_window().get_native_window());

  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::is_mouse_button_pressed_impl(int button) {
  auto window = static_cast<GLFWwindow*>(
      Application::get().get_window().get_native_window());

  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float WindowsInput::get_mouse_x_impl() {
  auto [x, y] = get_mouse_position_impl();
  return x;
}

float WindowsInput::get_mouse_y_impl() {
  auto [x, y] = get_mouse_position_impl();
  return y;
}

std::pair<float, float> WindowsInput::get_mouse_position_impl() {
  double xpos, ypos;
  auto window = static_cast<GLFWwindow*>(
      Application::get().get_window().get_native_window());

  glfwGetCursorPos(window, &xpos, &ypos);
  return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
}

}  // namespace ARIA
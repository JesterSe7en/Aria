#include "ariapch.h"
#include "WindowsInput.hpp"
#include "Aria/Core/Application.hpp"
#include "Aria/Core/Input.hpp"
#include <GLFW/glfw3.h>

namespace aria {

Input *Input::p_instance_ = new WindowsInput();

// keycode param is a GLFW keycode for now
bool WindowsInput::IsKeyPressedImpl(int keycode) {
  auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());

  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button) {
  auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());

  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float WindowsInput::GetMouseXImpl() {
  auto [x, y] = GetMousePositionImpl();
  return x;
}

float WindowsInput::GetMouseYImpl() {
  auto [x, y] = GetMousePositionImpl();
  return y;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl() {
  double x_pos, y_pos;
  auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());

  glfwGetCursorPos(window, &x_pos, &y_pos);
  return std::pair<float, float>(static_cast<float>(x_pos), static_cast<float>(y_pos));
}

}// namespace aria
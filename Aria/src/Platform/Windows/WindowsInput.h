#pragma once

#include "Aria/Core/Input.h"

namespace ARIA {

class WindowsInput : public Input {
 protected:
  bool is_key_pressed_impl(int keycode) override;
  bool is_mouse_button_pressed_impl(int button) override;
  float get_mouse_x_impl() override;
  float get_mouse_y_impl() override;
  std::pair<float, float> get_mouse_position_impl() override;
};
}  // namespace ARIA
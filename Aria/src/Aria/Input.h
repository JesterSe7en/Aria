#pragma once

#include "Aria/Core.h"

namespace ARIA {

class ARIA_API Input {
 public:
  inline static bool is_key_pressed(int keycode) {
    return sInstance->is_key_pressed_impl(keycode);
  }

  inline static bool is_mouse_button_pressed(int button) {
    return sInstance->is_mouse_button_pressed_impl(button);
  }

  inline static float get_mouse_x() { return sInstance->get_mouse_x_impl(); }

  inline static float get_mouse_y() { return sInstance->get_mouse_y_impl(); }

  inline static std::pair<float, float> get_mouse_position() {
    return sInstance->get_mouse_position_impl();
  }

  // These will be implemented per platform (Windows, MacOS, etc.)
 protected:
  virtual bool is_key_pressed_impl(int keycode) = 0;
  virtual bool is_mouse_button_pressed_impl(int button) = 0;
  virtual float get_mouse_x_impl() = 0;
  virtual float get_mouse_y_impl() = 0;
  virtual std::pair<float, float> get_mouse_position_impl() = 0;

 private:
  static Input* sInstance;
};

}  // namespace ARIA
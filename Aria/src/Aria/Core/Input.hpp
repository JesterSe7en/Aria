#pragma once

#include "Base.hpp"

namespace aria {

class ARIA_API Input {
 public:
  inline static bool IsKeyPressed(int keycode) { return p_instance_->IsKeyPressedImpl(keycode); }

  inline static bool IsMouseButtonPressed(int button) { return p_instance_->IsMouseButtonPressedImpl(button); }

  inline static float GetMouseX() { return p_instance_->GetMouseXImpl(); }

  inline static float GetMouseY() { return p_instance_->GetMouseYImpl(); }

  inline static std::pair<float, float> GetMousePosition() { return p_instance_->GetMousePositionImpl(); }

  // These will be implemented per platform (Windows, MacOS, etc.)
 protected:
  virtual bool IsKeyPressedImpl(int keycode) = 0;
  virtual bool IsMouseButtonPressedImpl(int button) = 0;
  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;
  virtual std::pair<float, float> GetMousePositionImpl() = 0;

 private:
  static Input *p_instance_;
};

}// namespace aria
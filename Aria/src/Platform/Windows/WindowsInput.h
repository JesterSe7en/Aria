#pragma once

#include "Aria/Core/Input.h"

namespace aria {

class WindowsInput : public Input {
 protected:
  bool IsKeyPressedImpl(int keycode) override;
  bool IsMouseButtonPressedImpl(int button) override;
  float GetMouseXImpl() override;
  float GetMouseYImpl() override;
  std::pair<float, float> GetMousePositionImpl() override;
};
}  // namespace ARIA
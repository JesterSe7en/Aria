#pragma once

#include "Aria/Layer.h"

namespace Aria {
class ARIA_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  void OnAttach() override;
  void OnDetach() override;

  void OnUpdate() override;
  void OnEvent(Event& event) override;

 private:
  const char* GLSL_VERSION = "#version 430 core";
  float m_Time = 0.0f;
};
}  // namespace Aria
#pragma once

#include "Aria/Layer.h"

namespace ARIA {
class ARIA_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  void on_attach() override;
  void on_detach() override;

  void on_update(Timestep delta_time) override;
  void on_event(Event& event) override;

 private:
  const char* GLSL_VERSION = "#version 430 core";
  float mTime = 0.0f;
};
}  // namespace ARIA
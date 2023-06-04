#pragma once

#include "Aria/Core/Layer.h"

namespace aria {
class ARIA_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event &event) override;
  void OnUpdate(Timestep delta_time) override;
  void OnImGuiRender() override;
  void Begin() const;
  void End() const;

 private:
  const char *glsl_version_ = "#version 430 core";
};
}// namespace aria
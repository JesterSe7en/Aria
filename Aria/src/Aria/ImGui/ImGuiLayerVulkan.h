#pragma once

#include "ImGuiLayer.h"

namespace aria {

class ImGuiLayerVulkan : public ImGuiLayer {
 public:
  ImGuiLayerVulkan();

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event &event) override;
  void OnUpdate(Timestep delta_time) override;
  void OnImGuiRender() override;
  void Begin() const override;
  void End() const override;
};
}// namespace aria
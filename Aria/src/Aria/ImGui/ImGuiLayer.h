#pragma once

#include "Aria/Core/Layer.h"

namespace ARIA {
class ARIA_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  void on_attach() override;
  void on_detach() override;
  void on_event(Event& event) override;

  void begin();
  void end();

 private:
  const char* GLSL_VERSION = "#version 430 core";
};
}  // namespace ARIA
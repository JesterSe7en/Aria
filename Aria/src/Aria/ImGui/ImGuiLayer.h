#pragma once

#include "Aria/Core/Layer.h"

namespace aria {

class ImGuiLayer : public Layer {
 public:
  explicit ImGuiLayer(const std::string &name);

  virtual void Begin() const = 0;
  virtual void End() const = 0;

  static Scope<ImGuiLayer> Create();
};

}// namespace aria
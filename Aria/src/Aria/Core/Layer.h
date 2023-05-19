#pragma once

#include "Base.h"
#include "Timestep.h"
#include "Aria/Events/Event.h"

namespace aria {
class ARIA_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer() = default;

  // TODO: Enable/disable layers
  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(Timestep delta_time) {}
  virtual void OnEvent(Event& event) {}
  virtual void OnImGuiRender() {}

  [[nodiscard]] inline const std::string& GetName() const { return debug_name_; }

 protected:
  std::string debug_name_;
};
}  // namespace ARIA
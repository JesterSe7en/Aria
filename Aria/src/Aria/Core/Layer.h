#pragma once

#include "Aria/Events/Event.h"
#include "Base.h"
#include "Timestep.h"

namespace aria {
class ARIA_API Layer {
 public:
  Layer(const std::string &name = "Layer");
  virtual ~Layer() = default;

  // TODO: Enable/disable layers
  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnUpdate(Timestep delta_time) = 0;
  virtual void OnEvent(Event &event) = 0;
  virtual void OnImGuiRender() = 0;

  inline const std::string &GetName() const { return debug_name; }

 protected:
  std::string debug_name;
};
}// namespace aria
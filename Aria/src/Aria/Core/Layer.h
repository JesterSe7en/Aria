#pragma once

#include "Base.h"
#include "Timestep.h"
#include "Aria/Events/Event.h"

namespace ARIA {
class ARIA_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer() = default;

  // TODO: Enable/disable layers
  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update(Timestep delta_time) {}
  virtual void on_event(Event& event) {}
  virtual void on_imgui_render() {}

  inline const std::string& get_name() const { return mDebugName; }

 protected:
  std::string mDebugName;
};
}  // namespace ARIA
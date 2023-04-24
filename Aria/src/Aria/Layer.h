#pragma once

#include "Aria/Core.h"
#include "Aria/Events/Event.h"

namespace ARIA {
class ARIA_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  // TODO: Enable/disable layers
  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update() {}
  virtual void on_event(Event& event) {}

  inline const std::string& get_name() const { return mDebugName; }

 protected:
  std::string mDebugName;
};
}  // namespace ARIA
#pragma once

#include "Aria/Core.h"
#include "Aria/Events/Event.h"

namespace Aria {
class ARIA_API Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  // TODO: Enable/disable layers
  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate() {}
  virtual void OnEvent(Event& event) {}

  inline const std::string& GetName() const { return m_DebugName; }

 protected:
  std::string m_DebugName;
};
}  // namespace Aria
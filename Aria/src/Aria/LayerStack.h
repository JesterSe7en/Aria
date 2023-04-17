#pragma once

#include "Aria/Core.h"
#include "Layer.h"

namespace Aria {
class ARIA_API LayerStack {
 public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
  std::vector<Layer*>::iterator end() { return m_Layers.end(); }

  // Render - forwards (0 to size - 1)
  // Events - backwards (size -1 to 0)

 private:
  std::vector<Layer*> m_Layers;
  std::vector<Layer*>::iterator m_LayerInsert;
  ;
};
}  // namespace Aria
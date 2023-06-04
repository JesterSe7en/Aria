#pragma once

#include "Base.h"
#include "Layer.h"

namespace aria {
class ARIA_API LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *overlay);

  std::vector<Layer *>::iterator begin() { return layers_.begin(); }
  std::vector<Layer *>::iterator end() { return layers_.end(); }

  // Render - forwards (0 to size - 1)
  // Events - backwards (size -1 to 0)

 private:
  std::vector<Layer *> layers_;
  unsigned int layer_insert_index_ = 0;
};
}// namespace aria
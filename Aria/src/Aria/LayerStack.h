#pragma once

#include "Aria/Core.h"
#include "Layer.h"

namespace ARIA {
class ARIA_API LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void push_layer(Layer* layer);
  void push_overlay(Layer* overlay);
  void pop_layer(Layer* layer);
  void pop_overlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return mLayers.begin(); }
  std::vector<Layer*>::iterator end() { return mLayers.end(); }

  // Render - forwards (0 to size - 1)
  // Events - backwards (size -1 to 0)

 private:
  std::vector<Layer*> mLayers;
  unsigned int mLayerInsertIndex = 0;
};
}  // namespace ARIA
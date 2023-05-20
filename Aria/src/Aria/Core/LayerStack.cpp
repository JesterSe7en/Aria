#include "ariapch.h"
#include "LayerStack.h"

namespace aria {

// Overlays are placed at the back of the vector
// Layers are place in the layer insert

// TODO: maybe use a std::deque structure
//  " A double-ended queue that provides efficient insertion and removal at both ends, similar to a vector, but with
//  potentially better performance for frequent insertions and removals at the beginning."

LayerStack::~LayerStack() {
  for (Layer *layer : layers_) {
    delete layer;
  }
}

void LayerStack::PushLayer(Layer *layer) {
  layers_.emplace(layers_.begin() + layer_insert_index_, layer);
  layer_insert_index_++;
}

void LayerStack::PushOverlay(Layer *overlay) { layers_.emplace_back(overlay); }

// In a sense you are "popping" but this isn't a real stack structure,
// realistically you are removing a layer

void LayerStack::PopLayer(Layer *layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);
  if (it != layers_.end()) {
    layers_.erase(it);
    layer_insert_index_--;
  }
}

void LayerStack::PopOverlay(Layer *overlay) {
  auto it = std::find(layers_.begin(), layers_.end(), overlay);
  if (it != layers_.end()) {
    layers_.erase(it);
  }
}
}  // namespace ARIA
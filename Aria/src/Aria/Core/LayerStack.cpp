#include "ariapch.h"
#include "LayerStack.h"

namespace ARIA {

// Overlays are placed at the back of the vector
// Layers are place in the layer insert

// TODO: maybe use a std::deque structure
//  " A double-ended queue that provides efficient insertion and removal at both ends, similar to a vector, but with
//  potentially better performance for frequent insertions and removals at the beginning."

LayerStack::~LayerStack() {
  for (Layer* layer : mLayers) {
    delete layer;
  }
}

void LayerStack::push_layer(Layer* layer) {
  mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
  mLayerInsertIndex++;
}

void LayerStack::push_overlay(Layer* overlay) { mLayers.emplace_back(overlay); }

// In a sense you are "popping" but this isn't a real stack structure,
// realistically you are removing a layer

void LayerStack::pop_layer(Layer* layer) {
  auto it = std::find(mLayers.begin(), mLayers.end(), layer);
  if (it != mLayers.end()) {
    mLayers.erase(it);
    mLayerInsertIndex--;
  }
}

void LayerStack::pop_overlay(Layer* overlay) {
  auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
  if (it != mLayers.end()) {
    mLayers.erase(it);
  }
}
}  // namespace ARIA
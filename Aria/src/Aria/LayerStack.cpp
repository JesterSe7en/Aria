#include "ariapch.h"
#include "LayerStack.h"

namespace ARIA {

// Overlays are placed at the back of the vector
// Layers are place in the layer insert

LayerStack::LayerStack() { mLayerInsert = mLayers.begin(); }

LayerStack::~LayerStack() {
  for (Layer* layer : mLayers) {
    delete layer;
  }
}

void LayerStack::push_layer(Layer* layer) {
  mLayerInsert = mLayers.emplace(mLayerInsert, layer);
}

void LayerStack::push_overlay(Layer* overlay) { mLayers.emplace_back(overlay); }

// In a sense you are "popping" but this isn't a real stack structure,
// realistically you are removing a layer

void LayerStack::pop_layer(Layer* layer) {
  auto it = std::find(mLayers.begin(), mLayers.end(), layer);
  if (it != mLayers.end()) {
    mLayers.erase(it);
    mLayerInsert--;
  }
}

void LayerStack::pop_overlay(Layer* overlay) {
  auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
  if (it != mLayers.end()) {
    mLayers.erase(it);
  }
}
}  // namespace ARIA
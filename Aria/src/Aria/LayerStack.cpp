#include "ariapch.h"
#include "LayerStack.h"

namespace Aria {

  // Overlays are placed at the back of the vector
  // Layers are place in the layer insert

LayerStack::LayerStack() { m_LayerInsert = m_Layers.begin(); }

LayerStack::~LayerStack() {
  for (Layer* layer : m_Layers) {
    delete layer;
  }
}

void LayerStack::PushLayer(Layer* layer) {
  m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

void LayerStack::PushOverlay(Layer* overlay) { m_Layers.emplace_back(overlay); }

// In a sense you are "popping" but this isn't a real stack structure, realistically you are removing a layer

void LayerStack::PopLayer(Layer* layer) {
  auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
  if (it != m_Layers.end()) {
    m_Layers.erase(it);
    m_LayerInsert--;
  }
}

void LayerStack::PopOverlay(Layer* overlay) {
  auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
  if (it != m_Layers.end()) {
    m_Layers.erase(it);
  }
}
}  // namespace Aria
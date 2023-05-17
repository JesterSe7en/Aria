#include "VulkanLayer.h"

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"

VulkanLayer::VulkanLayer() : Layer("Vulkan Layer") {
  // hi
  // mVertexShader =
}

void VulkanLayer::on_update(ARIA::Timestep delta_time) {}

void VulkanLayer::on_event(ARIA::Event& event) {}

void VulkanLayer::on_imgui_render() {}
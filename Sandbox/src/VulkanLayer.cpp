#include "VulkanLayer.h"

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/Shader.h"

VulkanLayer::VulkanLayer() : Layer("Vulkan Layer") {
  // hi
  // mVertexShader =

  mVertexShader =
      mShaderLibrary.load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/vert.spv", ARIA::ShaderType::VERTEX);
  mFragShader =
      mShaderLibrary.load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/frag.spv", ARIA::ShaderType::FRAGMENT);
  ARIA::Renderer::create_pipeline();
}

void VulkanLayer::on_update(ARIA::Timestep delta_time) {}

void VulkanLayer::on_event(ARIA::Event& event) {}

void VulkanLayer::on_imgui_render() {}

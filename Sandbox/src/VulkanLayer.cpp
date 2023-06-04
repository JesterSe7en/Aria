#include "VulkanLayer.h"
#include "Aria/Renderer/RenderCommand.h"

VulkanLayer::VulkanLayer() : Layer("Vulkan Layer") {
  vertex_shader_ =
      shader_library_.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/vert.spv", aria::ShaderType::VERTEX);
  frag_shader_ =
      shader_library_.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/frag.spv", aria::ShaderType::FRAGMENT);

  //TODO: see if we can limit the funciton calls here specificaly the sets and binding
  aria::RenderCommand::CreateCommandModule();
  // aria::RenderCommand::BeginRecording();
  // aria::RenderCommand::CmdBeginRenderPass();
  // aria::RenderCommand::CmdBindToGraphicsPipeline();
  // aria::RenderCommand::CmdSetViewport();
  // aria::RenderCommand::CmdSetScissor();
  // aria::RenderCommand::CmdDraw();
  // aria::RenderCommand::CmdEndRenderPass();
  // aria::RenderCommand::EndRecording();
}

void VulkanLayer::OnAttach() {}

void VulkanLayer::OnDetach() {}

void VulkanLayer::OnUpdate(aria::Timestep delta_time) { aria::RenderCommand::DrawFrame(); }

void VulkanLayer::OnEvent(aria::Event &event) {}

void VulkanLayer::OnImGuiRender() {}

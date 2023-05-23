#pragma once

#include "Aria/Core/Timestep.hpp"
#include "Aria/Events/Event.hpp"
#include "Aria/Renderer/Shader.hpp"
#include "Platform/Vulkan/VulkanCommandBuffer.hpp"  //TODO: Maybe - Refactor so that layers don't require this1
#include "Platform/Vulkan/VulkanRendererApi.hpp"
#include "vulkan/vulkan_core.h" // TODO: Refactor so that layers don't require this

#include <Aria.hpp>

class VulkanLayer : public aria::Layer {
 public:
  VulkanLayer();
  ~VulkanLayer() override;

  void OnUpdate(aria::Timestep delta_time) override;
  void OnEvent(aria::Event &event) override;

  void OnImGuiRender() override;

 private:
  aria::Ref<aria::Shader> vertex_shader_;
  aria::Ref<aria::Shader> frag_shader_;
  aria::ShaderLibrary shader_library_;
//  aria::VulkanCommandBuffer command_buffer_;

//  VkSemaphore image_available_semaphore_;
//  VkSemaphore render_finished_semaphore_;
//  VkFence in_flight_fence_;
};
#pragma once

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Shader.h"
#include <Aria.h>

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
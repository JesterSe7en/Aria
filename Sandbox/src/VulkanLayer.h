#pragma once

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Shader.h"
#include <Aria.h>

class VulkanLayer : public aria::Layer {
 public:
  VulkanLayer();
  ~VulkanLayer() override;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(aria::Timestep delta_time) override;
  void OnEvent(aria::Event &event) override;

  void OnImGuiRender() override;

 private:
  aria::Ref<aria::Shader> vertex_shader_;
  aria::Ref<aria::Shader> frag_shader_;
  aria::ShaderLibrary shader_library_;
};
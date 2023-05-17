#pragma once

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Shader.h"

#include <Aria.h>

class VulkanLayer : public ARIA::Layer {
 public:
  VulkanLayer();

  void on_update(ARIA::Timestep delta_time) override;
  void on_event(ARIA::Event& event) override;

  void on_imgui_render() override;

 private:
  ARIA::Ref<ARIA::Shader> mVertexShader;
  ARIA::Ref<ARIA::Shader> mFragShader;
  ARIA::ShaderLibrary mShaderLibrary;
};
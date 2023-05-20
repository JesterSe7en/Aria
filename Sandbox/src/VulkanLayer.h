#pragma once

#include "Aria/Core/Timestep.h"
#include "Aria/Events/Event.h"
#include "Aria/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#include "vulkan/vulkan_core.h"

#include <Aria.h>

class VulkanLayer : public aria::Layer {
 public:
  VulkanLayer();
  ~VulkanLayer() override;

  void OnUpdate(aria::Timestep delta_time) override;
  void OnEvent(aria::Event &event) override;

  void OnImGuiRender() override;

  static VkDevice sDevice;

 private:
  aria::Ref<aria::Shader> mVertexShader;
  aria::Ref<aria::Shader> mFragShader;
  aria::ShaderLibrary mShaderLibrary;

  VkSemaphore mImageAvailableSemaphore;
  VkSemaphore mRenderFinishedSemaphore;
  VkFence mInFlightFence;
  aria::VulkanCommandBuffer mCommandBuffer;
};
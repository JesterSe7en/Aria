#pragma once

#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/RendererApi.h"
#include "Aria/Renderer/Shader.h"
#include "VulkanDebugMessenger.h"
#include "VulkanDeviceManager.h"
#include "VulkanInstance.h"
#include "vulkan/vulkan.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace aria {

class VulkanRendererApi : public RendererApi {
 public:
  VulkanRendererApi();
  ~VulkanRendererApi();

  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  VulkanRendererApi(const VulkanRendererApi &) = delete;           // copy constructor
  VulkanRendererApi &operator=(const VulkanRendererApi &) = delete;// copy assignment
  VulkanRendererApi(VulkanRendererApi &&) = delete;                //move constructor
  VulkanRendererApi &operator=(VulkanRendererApi &&) = delete;     //move assignment

  void CreatePipeline() override;

  static VulkanRendererApi &GetInstance() {
    static VulkanRendererApi instance;
    return instance;
  }

  void AddToPipeline(VkShaderModule &shader_module, ShaderType type);

 private:
  Ref<VulkanInstance> p_vulkan_instance_;

  VkInstance p_vk_instance_;
  VkSurfaceKHR surface_;
  VkCommandPool command_pool_;
  std::vector<VkCommandBuffer> command_buffers_;
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkQueue present_queue_;
  VkQueue graphics_queue_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  void CreateCommandPool();
  void CreateCommandBuffer();
};
}// namespace aria

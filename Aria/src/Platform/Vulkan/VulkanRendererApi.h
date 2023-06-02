#pragma once

#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/RendererApi.h"
#include "Aria/Renderer/Shader.h"
#include "VulkanDebugMessenger.h"
#include "VulkanDeviceManager.h"
#include "VulkanInstance.h"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"
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

  //  static VulkanRendererApi &GetInstance() {
  //    static VulkanRendererApi instance_;
  //    return instance_;
  //  }

  //  VkSurfaceKHR &GetVkSurfaceKhr() { return surface_; }
  //  Ref<VulkanInstance> GetVulkanInstance() const { return p_vulkan_instance_; }

  //  void AddToPipeline(VkShaderModule &shader_module, ShaderType type);
  //  void CreatePipeline() override;

  constexpr static const std::array<const char *, 1> kValidationLayers = {"VK_LAYER_KHRONOS_validation"};

 private:
  Ref<VulkanInstance> p_vulkan_instance_;

  VkInstance p_vk_instance_;
  VkSurfaceKHR surface_;
  //  VkRenderPass vk_render_pass_;
  //  VkCommandPool command_pool_;
  //  VkCommandBuffer command_buffer_;
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkQueue present_queue_;
  VkQueue graphics_queue_;

  //  VulkanDebugMessenger vulkan_debug_messenger_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  //  void CreateRenderPass();
  //  void CreateCommandPool();
  //  static bool HasValidationSupport();
};
}// namespace aria

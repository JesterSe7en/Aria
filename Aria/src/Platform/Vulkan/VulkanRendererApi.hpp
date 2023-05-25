#pragma once

#include "Aria/Renderer/Camera.hpp"
#include "Aria/Renderer/RendererApi.hpp"
#include "Aria/Renderer/Shader.hpp"
#include "VulkanDebugMessenger.hpp"
#include "VulkanDeviceManager.hpp"
#include "VulkanInstance.hpp"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace aria {

class VulkanRendererApi : public RendererApi {
 public:
  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  VulkanRendererApi (const VulkanRendererApi &) = delete; // copy constructor
  VulkanRendererApi &operator=(const VulkanRendererApi &) = delete; // copy assignment
  VulkanRendererApi (VulkanRendererApi &&) = delete; //move constructor
  VulkanRendererApi &operator=(VulkanRendererApi &&) = delete; //move assignment

  static VulkanRendererApi& GetInstance() {
    static VulkanRendererApi instance_;
    return instance_;
  }

  VkSurfaceKHR &GetVkSurfaceKhr() { return surface_; }
  Ref<VulkanInstance> GetVulkanInstance() const { return p_vulkan_instance_; }

  void AddToPipeline(VkShaderModule &shader_module, ShaderType type);
  void CreatePipeline() override;

  constexpr static const std::array<const char *, 1> kValidationLayers = {"VK_LAYER_KHRONOS_validation"};

 private:
  VulkanRendererApi();
  ~VulkanRendererApi();

  Ref<VulkanInstance> p_vulkan_instance_;

  VkInstance p_vk_instance_;
  VkSurfaceKHR surface_;
  VkRenderPass vk_render_pass_;
  VkCommandPool command_pool_;
  VkCommandBuffer command_buffer_;
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkQueue present_queue_;
  VkQueue graphics_queue_;

  VulkanDebugMessenger vulkan_debug_messenger_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  void CreatePresentationSurface();
  //  void CreateRenderPass();
  void CreateCommandPool();
  static bool HasValidationSupport();
  static std::vector<const char *> GetGlfwRequiredExtensions();
};
}// namespace aria

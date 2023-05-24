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
  ~VulkanRendererApi();

  static VulkanRendererApi GetInstance() {
    static VulkanRendererApi instance;
    return instance;
  }

  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  static bool IsValidationLayersEnabled() {
#ifdef ARIA_DEBUG
    return true;
#else
    return false;
#endif
  };

  VkCommandBuffer CreateVkCommandBuffer();
  VkCommandBuffer GetVkCommandBuffer() const {
    return command_buffer_;
  }// this is a hack for now, really should not do this.  perhaps create static func to create buffer for layer usage

  //  static void RecordCommandBuffer(VkCommandBuffer cmd_buffer,
  //                                  std::uint32_t image_idx);  // again a hack, should put this in vulkan layer

  VkInstance &GetVkInstance() {
    ARIA_CORE_ASSERT(VulkanRendererApi::p_vk_instance_, "Did you initialize a Vulkan instance?")
    return p_vk_instance_;
  }
  VkCommandPool &GetVkCommandPool() { return command_pool_; }
  VkSurfaceKHR &GetVkSurfaceKhr() { return surface_; }
  VkRenderPass &GetRenderPass() { return vk_render_pass_; }

  void AddToPipeline(VkShaderModule &shader_module, ShaderType type);
  void CreatePipeline() override;

  constexpr static const std::array<const char *, 1> validation_layers_ = {"VK_LAYER_KHRONOS_validation"};

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

  void CreateInstance();
  Ref<VulkanInstance> p_vulkan_instance_;
  void CreatePresentationSurface();
  //  void CreateRenderPass();
  void CreateCommandPool();
  static bool HasValidationSupport();
  static std::vector<const char *> GetGlfwRequiredExtensions();
};
}// namespace aria

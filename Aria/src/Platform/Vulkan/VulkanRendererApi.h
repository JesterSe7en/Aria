#pragma once

#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/RendererApi.h"
#include "Aria/Renderer/Shader.h"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"
#include "VulkanDebugMessenger.h"
#include "VulkanDeviceManager.h"

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace aria {

class VulkanRendererApi : public RendererApi {
 public:
  ~VulkanRendererApi();

  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  void CreatePipeline() override;

  static VkInstance &GetVkInstance() {
    ARIA_CORE_ASSERT(VulkanRendererApi::p_vk_instance_, "Did you initialize a Vulkan instance?")
    return p_vk_instance_;
  }

  static bool IsValidationLayersEnabled() {
#ifdef ARIA_DEBUG
    return true;
#else
    return false;
#endif
  };

  static VkCommandBuffer CreateVkCommandBuffer();
  static VkCommandBuffer GetVkCommandBuffer() {
    return command_buffer_;
  }  // this is a hack for now, really should not do this.  perhaps create static func to create buffer for layer usage

  static void RecordCommandBuffer(VkCommandBuffer cmd_buffer,
                                  std::uint32_t image_idx);  // again a hack, should put this in vulkan layer

  static VkCommandPool GetVkCommandPool() { return command_pool_; }
  static VkSurfaceKHR &GetVkSurfaceKHR() { return surface_; }
  static VkRenderPass &GetRenderPass() { return vk_render_pass_; }

  static void AddToPipeline(VkShaderModule &shader_module, ShaderType type);

  constexpr static const std::array<const char *, 1> validation_layers_ = {"VK_LAYER_KHRONOS_validation"};

  static VkInstance p_vk_instance_;
  static VkSurfaceKHR surface_;
  static VkRenderPass vk_render_pass_;
  static VkCommandPool command_pool_;
  static VkCommandBuffer command_buffer_;
  static std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkQueue present_queue_;
  VkQueue graphics_queue_;

  VulkanDebugMessenger vulkan_debug_messenger_;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  static void CreateInstance();
  static void CreatePresentationSurface();
  static void CreateRenderPass();
  static void CreateCommandPool();
  void CreateCommandBuffer();

  static bool HasValidationSupport();
  static std::vector<const char *> GetGlfwRequiredExtensions();

  void Cleanup();
};
}  // namespace ARIA

#pragma once

#include "VulkanRenderPass.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanGraphicsPipeline {
 public:
  static VulkanGraphicsPipeline &GetInstance() {
    static VulkanGraphicsPipeline instance;
    return instance;
  }

  ~VulkanGraphicsPipeline();
  VulkanGraphicsPipeline(const VulkanGraphicsPipeline &) = delete;
  VulkanGraphicsPipeline &operator=(const VulkanGraphicsPipeline &) = delete;

  void Init();
  void CreateGraphicsPipeline();
  void CreateFrameBuffers();
  VkPipeline GetGraphicsPipeline() const;
  std::vector<VkFramebuffer> GetFrameBuffers() const;

 private:
  VulkanGraphicsPipeline() = default;

  constexpr static const std::array<VkDynamicState, 2> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT,
                                                                          VK_DYNAMIC_STATE_SCISSOR};

  Ref<VulkanRenderPass> vk_render_pass_;
  std::vector<VkFramebuffer> vk_frame_buffers_;
  VkPipelineLayout vk_pipeline_layout_;
  VkPipeline vk_graphics_pipeline_;
  static std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;
};

}// namespace aria

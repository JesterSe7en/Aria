#pragma once

#include "Aria/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "VulkanRenderPass.h"
#include "vulkan/vulkan_core.h"

namespace aria {

class VulkanGraphicsPipeline {
 public:
  static VulkanGraphicsPipeline &GetInstance() {
    static VulkanGraphicsPipeline instance_;
    return instance_;
  }

  ~VulkanGraphicsPipeline();
  VulkanGraphicsPipeline(const VulkanGraphicsPipeline &) = delete;
  VulkanGraphicsPipeline &operator=(const VulkanGraphicsPipeline &) = delete;

  void Init();
  void CreateGraphicsPipeline();
  void CreateFrameBuffers();
  void AddToShaderStages(VkShaderModule &shader_module, ShaderType type);
  VkPipeline GetGraphicsPipeline() const { return vk_graphics_pipeline_; }
  std::vector<VkFramebuffer> GetFrameBuffers() const { return vk_frame_buffers_; }
  Ref<VulkanRenderPass> GetRenderPass() const { return vk_render_pass_; }

 private:
  VulkanGraphicsPipeline();

  constexpr static const std::array<VkDynamicState, 2> kDynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                                                         VK_DYNAMIC_STATE_SCISSOR};
  //  void InitPipelineCache();
  //  void UpdatePipeline();
  void DestroyPipeline();
  static bool IsAllModulesSet();

  Ref<VulkanRenderPass> vk_render_pass_;
  std::vector<VkFramebuffer> vk_frame_buffers_;
  VkPipelineLayout vk_pipeline_layout_;
  VkPipeline vk_graphics_pipeline_;
  //  VkPipelineCache vk_pipeline_cache_;
  static std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;
};

}// namespace aria

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
  VulkanRendererApi() = default;
  ~VulkanRendererApi();

  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  void CreateCommandModule() override;//FIXME: hack to encapsulate the entire command buffer recording
  void BeginRecording() override;
  void EndRecording() override;
  void CmdBeginRenderPass() override;
  void CmdEndRenderPass() override;
  void CmdBindToGraphicsPipeline() override;
  void CmdSetViewport() override;
  void CmdSetScissor() override;
  void CmdDraw() override;
  void DrawFrame() override;

  VulkanRendererApi(const VulkanRendererApi &) = delete;           // copy constructor
  VulkanRendererApi &operator=(const VulkanRendererApi &) = delete;// copy assignment
  VulkanRendererApi(VulkanRendererApi &&) = delete;                //move constructor
  VulkanRendererApi &operator=(VulkanRendererApi &&) = delete;     //move assignment

  static VulkanRendererApi &GetInstance() {
    static VulkanRendererApi instance_;
    return instance_;
  }

  Ref<VulkanInstance> GetVkInstance() { return p_vulkan_instance_; }
  Ref<VulkanDeviceManager> GetVkDeviceManager() { return p_vulkan_device_manager_; }

  VkQueue &GetPresentQueue() { return present_queue_; }
  VkQueue &GetGraphicsQueue() { return graphics_queue_; }
  std::vector<VkCommandBuffer> &GetCommandBuffers() { return command_buffers_; }
  void AddToPipeline(VkShaderModule &shader_module, ShaderType type);

 private:
  Ref<VulkanInstance> p_vulkan_instance_;
  Ref<VulkanDeviceManager> p_vulkan_device_manager_;

  VkInstance p_vk_instance_;
  VkSurfaceKHR surface_;
  VkCommandPool command_pool_;
  std::vector<VkCommandBuffer> command_buffers_;
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkQueue present_queue_;
  VkQueue graphics_queue_;

  uint32_t current_frame_idx = 0;

  // sync objects
  std::vector<VkSemaphore> available_semaphores_;
  std::vector<VkSemaphore> finished_semaphore_;
  std::vector<VkFence> in_flight_fences_;
  std::vector<VkFence> image_in_flight_;

  const int max_frames_in_flight_ = 2;

  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  void CreateCommandPool();
  void CreateCommandBuffer();
  void CreateSyncObjects();
  void GetQueues();
};
}// namespace aria

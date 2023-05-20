#pragma once

#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/RendererApi.h"
#include "Aria/Renderer/Shader.h"
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

  void Init() override;
  void Clear() override;
  void SetClearColor(const glm::vec4 color) override;
  void DrawIndexed(const Ref<VertexArray> &vertex_array) override;

  void CreatePipeline() override;

  static VkInstance &GetVkInstance() {
    ARIA_CORE_ASSERT(VulkanRendererApi::p_vk_instance_t_, "Did you initialize a Vulkan instance?")
    return p_vk_instance_t_;
  }

  static VkDevice GetVkDevice() { return p_device_t_; }
  static VkSwapchainKHR GetVkSwapChain() { return swapchain_khr_; }
  static VkCommandBuffer CreateVkCommandBuffer();
  static VkCommandBuffer GetVkCommandBuffer() {
    return command_buffer_;
  }  // this is a hack for now, really should not do this.  perhaps create static func to create buffer for layer usage

  static void RecordCommandBuffer(VkCommandBuffer cmd_buffer,
                                  std::uint32_t image_idx);  // again a hack, should put this in vulkan layer

  static VkCommandPool GetVkCommandPool() { return command_pool_; }
  static VkPipeline GetVkGraphicsPipeline() { return graphics_pipeline_; }

  static void AddToPipeline(VkShaderModule &shader_module, ShaderType type);

 private:
  struct QueryFamilyIndices {
    std::optional<std::uint32_t> graphics_family;
    std::optional<std::uint32_t> present_family;
    bool IsComplete() const { return graphics_family.has_value() && present_family.has_value(); }
  };

  struct SwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  static VkInstance p_vk_instance_t_;
  static VkDevice p_device_t_;
  static VkPipeline graphics_pipeline_;
  static VkSwapchainKHR swapchain_khr_;
  static VkCommandPool command_pool_;
  static VkCommandBuffer command_buffer_;
  static std::vector<VkShaderModule> shader_modules_;
  static std::vector<VkPipelineShaderStageCreateInfo> shader_stages_;

  VkDebugUtilsMessengerEXT debug_messenger_;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkQueue present_queue_;
  VkQueue graphics_queue_;
  VkSurfaceKHR surface_;

  VkFormat swap_chain_format_;
  VkExtent2D swap_chain_extent_2d_;
  VkRenderPass render_pass_;
  VkPipelineLayout pipeline_layout_;

  std::vector<VkImage> swap_chain_images_;
  std::vector<VkImageView> swap_chain_image_views_;
  std::vector<VkFramebuffer> swap_chain_frame_buffers_;

  const std::vector<const char *> validation_layers_ = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> device_extensions_ = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamic_states_ = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  void CreateInstance();
  void SetupVulkanDebugMessenger();
  void CreatePresentationSurface();
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateSwapChain();
  void CreateImageViews();
  void CreateRenderPass();
  void CreateGraphicsPipeline();
  void CreateFrameBuffers();
  void CreateCommandPool();
  void CreateCommandBuffer();

  bool HasValidationSupport() const;
  void PopulateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info) const;
  VkResult CreateDebugUtilMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                       const VkAllocationCallbacks *p_allocator,
                                       VkDebugUtilsMessengerEXT *p_debug_messenger);
  static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanLogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                          VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                          const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
                                                          void *p_user_data);
  static std::string GetMessageType(VkDebugUtilsMessageTypeFlagsEXT message_type);
  bool IsSuitableVulkanDevice(VkPhysicalDevice device);
  std::string GetVendorName(uint32_t vendor_id) const;
  QueryFamilyIndices QueryQueueFamilies(VkPhysicalDevice device);
  SwapChainDetails QuerySwapChainSupport(VkPhysicalDevice device);
  std::vector<const char *> GetGlfwRequiredExtensions();
  bool CheckDeviceExtensionsSupport(VkPhysicalDevice device);

  // ----- For Swap Chain ------
  VkSurfaceFormatKHR GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
  VkPresentModeKHR GetPresentMode(const std::vector<VkPresentModeKHR> &modes);
  VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  void Cleanup();
};
}  // namespace ARIA

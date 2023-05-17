#pragma once

#include "Aria/Renderer/RendererAPI.h"
#include "Aria/Renderer/Shader.h"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace ARIA {
class VulkanRendererAPI : public RendererAPI {
 public:
  ~VulkanRendererAPI();

  void init() override;
  void clear() override;
  void set_clear_color(const glm::vec4 color) override;
  void draw_indexed(const Ref<VertexArray>& vertex_array) override;

  void create_pipeline() override;

  static VkInstance get_vk_instance() {
    ARIA_CORE_ASSERT(VulkanRendererAPI::sInstance, "Did you initialize a Vulkan instance?")
    return sInstance;
  }

  static VkDevice get_vk_device() { return sDevice; }

  static void add_to_pipeline(VkShaderModule& shader_module, ShaderType type);

 private:
  struct QueryFamilyIndices {
    std::optional<std::uint32_t> graphicsFamily;
    std::optional<std::uint32_t> presentFamily;
    bool is_complete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

  struct SwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  static VkInstance sInstance;
  static VkDevice sDevice;
  static std::vector<VkShaderModule> sShaderModules;
  static std::vector<VkPipelineShaderStageCreateInfo> sShaderStages;

  VkDebugUtilsMessengerEXT mDebugMessenger;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkQueue mPresentQueue;
  VkQueue mGraphicsQueue;
  VkSurfaceKHR mSurface;
  VkSwapchainKHR mSwapChain;
  VkFormat mSwapChainFormat;
  VkExtent2D mSwapChainExtent;
  VkRenderPass mRenderPass;
  VkPipelineLayout mPipelineLayout;
  VkPipeline mGraphicsPipeline;

  std::vector<VkImage> mSwapChainImages;
  std::vector<VkImageView> mSwapChainImageViews;
  std::vector<VkFramebuffer> mSwapChainFrameBuffers;

  const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char*> mDeviceExtensions = {"VK_KHR_swapchain"};
  const std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  void create_instance();
  void setup_vulkan_debug_messenger();
  void create_presentation_surface();
  void pick_physical_device();
  void create_logical_device();
  void create_swap_chain();
  void create_image_views();
  void create_render_pass();
  void create_graphics_pipeline();
  void create_frame_buffers();

  bool has_validation_support() const;
  void populate_debug_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info) const;
  VkResult create_debug_util_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
                                           const VkAllocationCallbacks* p_allocator,
                                           VkDebugUtilsMessengerEXT* p_debug_messenger);
  static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_log_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                            VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
                                                            void* p_user_data);
  static std::string get_message_type(VkDebugUtilsMessageTypeFlagsEXT message_type);
  bool is_suitable_vulkan_device(VkPhysicalDevice device);
  std::string get_vendor_name(uint32_t vendor_id) const;
  QueryFamilyIndices query_queue_families(VkPhysicalDevice device);
  SwapChainDetails query_swap_chain_support(VkPhysicalDevice device);
  std::vector<const char*> get_glfw_required_extensions();
  bool check_device_extensions_support(VkPhysicalDevice device);

  // ----- For Swap Chain ------
  VkSurfaceFormatKHR get_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& formats);
  VkPresentModeKHR get_present_mode(const std::vector<VkPresentModeKHR>& modes);
  VkExtent2D get_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

  void cleanup();
};
}  // namespace ARIA

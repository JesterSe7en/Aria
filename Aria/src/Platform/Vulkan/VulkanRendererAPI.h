#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <optional>

#include "Aria/Renderer/RendererAPI.h"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"

namespace ARIA {
class VulkanRendererAPI : public RendererAPI {
 public:
  void init() override;
  void clear() override;
  void set_clear_color(const glm::vec4 color) override;
  void draw_indexed(const Ref<VertexArray>& vertex_array) override;

  VkInstance get_vk_instance() { return mInstance; }

 private:
  struct QueryFamilyIndicies {
    std::optional<uint32_t> mGraphicsFamily;
    bool is_complete() const { return mGraphicsFamily.has_value(); }
  };
  VkInstance mInstance;
  VkDebugUtilsMessengerEXT mDebugMessenger;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mLogicalDevice = VK_NULL_HANDLE;
  VkQueue mGraphicsQueue;
  const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};

  void create_instance();
  void setup_vulkan_debug_messenger();
  void pick_physical_device();
  void create_logical_device();
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
  QueryFamilyIndicies find_queue_families(VkPhysicalDevice device);
};
}  // namespace ARIA
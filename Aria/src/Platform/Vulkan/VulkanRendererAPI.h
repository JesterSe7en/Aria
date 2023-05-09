#pragma once

#include <glm/glm.hpp>

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

 private:
  void create_instance();
  void setup_vulkan_debug_messenger();
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

  const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
  VkInstance mInstance;
  VkDebugUtilsMessengerEXT mDebugMessenger;
};
}  // namespace ARIA
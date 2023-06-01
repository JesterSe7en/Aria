#include "VulkanSwapChain.h"
#include "ariapch.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "VulkanDebugMessenger.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanLib.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_core.h"
#include <fileapi.h>
#include <vector>

namespace aria {

VulkanRendererApi::VulkanRendererApi() {
  p_vulkan_instance_ = nullptr;
  surface_ = VK_NULL_HANDLE;
  //  vk_render_pass_ = VK_NULL_HANDLE;
  //  command_pool_ = VK_NULL_HANDLE;
  //  command_buffer_ = VK_NULL_HANDLE;
  p_vk_instance_ = nullptr;

  //  std::vector<const char *> required_extensions = GetGlfwRequiredExtensions();
  //  create_info.extension_count = required_extensions.size();
  //  create_info.pp_extension_names = required_extensions;
  VulkanInstance::VulkanInstanceCreateInfo create_info;
#ifdef NDEBUG
  create_info.enable_validation = false;
#else
  create_info.enable_validation = true;
#endif
  create_info.layer_count = 0;

  p_vulkan_instance_ = VulkanInstance::Create(create_info);
  p_vk_instance_ = p_vulkan_instance_->GetVKBInstance().instance;
  VulkanDeviceManager::GetInstance().Init(p_vulkan_instance_);

  //  VulkanGraphicsPipeline::GetInstance().Init();
}

VulkanRendererApi::~VulkanRendererApi() {
  //  vkDestroyCommandPool(VulkanDeviceManager::GetInstance().GetLogicalDevice(), command_pool_, nullptr);
  //  vkDestroyRenderPass(VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_render_pass_, nullptr);
  //  vkDestroySurfaceKHR(p_vk_instance_, surface_, nullptr);
  //  vkDestroyInstance(p_vk_instance_, nullptr);
}

void VulkanRendererApi::Init() { auto instance = new VulkanRendererApi(); }
void VulkanRendererApi::Clear() { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::SetClearColor(const glm::vec4 color) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::DrawIndexed(const Ref<VertexArray> &vertex_array) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::CreatePipeline() { ARIA_CORE_ASSERT(false, "Not Implemented") }

//void VulkanRendererApi::AddToPipeline(VkShaderModule &shader_module, ShaderType type) {
//  VkPipelineShaderStageCreateInfo create_info;
//  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//  create_info.flags = 0;
//  create_info.pNext = nullptr;
//
//  switch (type) {
//    case ShaderType::VERTEX:
//      create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
//      break;
//    case ShaderType::FRAGMENT:
//      create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//      break;
//    default:
//      ARIA_CORE_ASSERT(false, "Unknown shader type; cannot create VkShaderModule")
//      break;
//  }
//  create_info.module = shader_module;
//  create_info.pName = "main";
//  create_info.pSpecializationInfo = nullptr;
//  shader_stages_.push_back(create_info);
//}

//void VulkanRendererApi::CreatePipeline() { VulkanGraphicsPipeline::GetInstance().Init(); }

void VulkanRendererApi::CreatePresentationSurface() {
  ARIA_CORE_ASSERT(p_vk_instance_, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  VkResult result = glfwCreateWindowSurface(p_vk_instance_, glfw_window, nullptr, &surface_);
  if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Cannot create vulkan surface - {0}", string_VkResult(result)) }
}

//void VulkanRendererApi::CreateCommandPool() {
//
//  VulkanDeviceManager::QueueFamilyIndices queue_family_indices =
//      VulkanDeviceManager::GetInstance().GetQueueFamilyIndices();
//
//  VkCommandPoolCreateInfo cmd_pool_info;
//  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//  cmd_pool_info.flags = 0;
//  cmd_pool_info.pNext = nullptr;
//  cmd_pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
//
//  VkResult result = vkCreateCommandPool(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &cmd_pool_info, nullptr,
//                                        &command_pool_);
//  if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Failed to create command pool - {0}", string_VkResult(result)) }
//}
//
//VkCommandBuffer VulkanRendererApi::CreateVkCommandBuffer() {
//  VkCommandBufferAllocateInfo buffer_alloc_info;
//  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//  buffer_alloc_info.pNext = nullptr;
//  buffer_alloc_info.commandPool = command_pool_;
//  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//  buffer_alloc_info.commandBufferCount = 1;
//
//  VkResult result = vkAllocateCommandBuffers(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &buffer_alloc_info,
//                                             &command_buffer_);
//  if (result != VK_SUCCESS) {
//    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
//    return nullptr;
//  }
//  return command_buffer_;
//}
//
//bool VulkanRendererApi::HasValidationSupport() {
//  // how many instance layers can the vulkan system support?
//  std::uint32_t layer_count;
//  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
//
//  std::vector<VkLayerProperties> available_layers(layer_count);
//  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
//
//  for (const char *layer_name : kValidationLayers) {
//    bool layer_found = false;
//
//    for (const auto &kLayerProperties : available_layers) {
//      if (strcmp(layer_name, kLayerProperties.layerName) == 0) {
//        layer_found = true;
//        break;
//      }
//    }
//
//    if (!layer_found) { return false; }
//  }
//  return true;
//}
//
//std::vector<const char *> VulkanRendererApi::GetGlfwRequiredExtensions() {
//  std::uint32_t glfw_extension_count = 0;
//  const char **glfw_extensions;
//  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
//
//  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
//
//  if (VulkanRendererApi::HasValidationSupport()) { extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }
//  return extensions;
//}

}// namespace aria

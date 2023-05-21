#include "Platform/Vulkan/VulkanRendererApi.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_core.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"

#include <fileapi.h>
#include <array>
#include <vector>

namespace aria {

VkInstance VulkanRendererApi::p_vk_instance_ = nullptr;
std::vector<VkPipelineShaderStageCreateInfo> VulkanRendererApi::shader_stages_ = {};

VulkanRendererApi::~VulkanRendererApi() {
  vkDestroyCommandPool(VulkanDeviceManager::GetInstance().GetLogicalDevice(), command_pool_, nullptr);
  vkDestroyRenderPass(VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_render_pass_, nullptr);
  vkDestroySurfaceKHR(p_vk_instance_, surface_, nullptr);
  vkDestroyInstance(p_vk_instance_, nullptr);
}

void VulkanRendererApi::Init() {
  CreateInstance();
  CreatePresentationSurface();

  vulkan_debug_messenger_.Init();
  VulkanDeviceManager::GetInstance().Init();

  VulkanSwapChain::GetInstance().CreateSwapChain();
  VulkanSwapChain::GetInstance().CreateImageViews();

  CreateRenderPass();
  CreateCommandPool();
  // create_graphics_pipeline();
}
void VulkanRendererApi::Clear() {ARIA_CORE_ASSERT(false, "Not Implemented")}

void VulkanRendererApi::SetClearColor(const glm::vec4 color) {ARIA_CORE_ASSERT(false, "Not Implemented")}

void VulkanRendererApi::DrawIndexed(const Ref<VertexArray> &vertex_array) {
  ARIA_CORE_ASSERT(false, "Not Implemented")
}

void VulkanRendererApi::AddToPipeline(VkShaderModule &shader_module, ShaderType type) {
  VkPipelineShaderStageCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  create_info.flags = 0;
  create_info.pNext = nullptr;

  switch (type) {
    case ShaderType::VERTEX:
      create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case ShaderType::FRAGMENT:
      create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
    default: ARIA_CORE_ASSERT(false, "Unknown shader type; cannot create VkShaderModule")
      break;
  }
  create_info.module = shader_module;
  create_info.pName = "main";
  create_info.pSpecializationInfo = nullptr;
  shader_stages_.push_back(create_info);
}

void VulkanRendererApi::CreatePipeline() {
  VulkanGraphicsPipeline::GetInstance().Init();
}

void VulkanRendererApi::CreateInstance() {
  if (VulkanRendererApi::IsValidationLayersEnabled() && !HasValidationSupport()) {
    ARIA_CORE_WARN("Vulkan validation layers requested, but none available")
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Aria Vulkan Application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Aria";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_VERSION_1_3;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  std::vector<const char *> extensions = GetGlfwRequiredExtensions();

  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = 0;

  // setup another create info struct to capture events during creation and destruction of VKInstance
  // Vulkan 1.3 spec pg. 3921
  if (VulkanRendererApi::IsValidationLayersEnabled()) {
    create_info.enabledLayerCount = validation_layers_.size();
    create_info.ppEnabledLayerNames = validation_layers_.data();
    create_info.pNext = &VulkanDebugMessenger::GetDebugMessengerCreateInfo();
  } else {
    create_info.enabledLayerCount = 0;
    create_info.pNext = nullptr;
  }

  // query the supported instance extensions
  // std::uint32_t numInstanceExtensions = 0;
  // std::vector<VkExtensionProperties> instanceExtensionProperties;
  // Query the instance extensions.
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, nullptr);

  // If there are any extensions, query their properties.

  // if (numInstanceExtensions != 0) {
  // instanceExtensionProperties.resize(numInstanceExtensions);
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, instanceExtensionProperties.data());
  // }
  // if instance extensions are enabled, look for function pointers via
  // vkGetInstanceProcAddr() func

  VkResult result = vkCreateInstance(&create_info, nullptr, &p_vk_instance_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreatePresentationSurface() {
  ARIA_CORE_ASSERT(p_vk_instance_, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  VkResult result = glfwCreateWindowSurface(p_vk_instance_, glfw_window, nullptr, &surface_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create vulkan surface - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreateCommandPool() {

  VulkanDeviceManager::QueueFamilyIndices
      queue_family_indices = VulkanDeviceManager::GetInstance().GetQueueFamilyIndicies();

  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = 0;
  cmd_pool_info.pNext = nullptr;
  cmd_pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

  VkResult
      result = vkCreateCommandPool(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &cmd_pool_info, nullptr, &command_pool_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command pool - {0}", string_VkResult(result))
  }
}

VkCommandBuffer VulkanRendererApi::CreateVkCommandBuffer() {
  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = command_pool_;
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = 1;

  VkResult
      result = vkAllocateCommandBuffers(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &buffer_alloc_info, &command_buffer_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
    return nullptr;
  }
  return command_buffer_;
}

bool VulkanRendererApi::HasValidationSupport() {
  // how many instance layers can the vulkan system support?
  std::uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char *layer_name : validation_layers_) {
    bool layer_found = false;

    for (const auto &kLayerProperties : available_layers) {
      if (strcmp(layer_name, kLayerProperties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      return false;
    }
  }
  return true;
}

void VulkanRendererApi::CreateRenderPass() {
  VkAttachmentDescription color_attachment;
  color_attachment.format = VulkanSwapChain::GetInstance().GetSwapChainDetails().swap_chain_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref;
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass;
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_info;
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;

  VkResult
      result =
      vkCreateRenderPass(VulkanDeviceManager::GetInstance().GetLogicalDevice(), &render_pass_info, nullptr, &vk_render_pass_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create render pass - {0}", string_VkResult(result))
  }
}

std::vector<const char *> VulkanRendererApi::GetGlfwRequiredExtensions() {
  std::uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  if (VulkanRendererApi::HasValidationSupport()) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}
}  // namespace ARIA

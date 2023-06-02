#include "ariapch.h"
#include "VulkanRendererApi.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "VulkanDebugMessenger.h"
#include "VulkanError.h"
#include "VulkanGraphicsPipeline.h"
#include <vector>

namespace aria {

VulkanRendererApi::VulkanRendererApi() {
  p_vulkan_instance_ = nullptr;
  surface_ = VK_NULL_HANDLE;
  p_vk_instance_ = nullptr;

  VulkanInstance::VulkanInstanceCreateInfo create_info;
#ifdef NDEBUG
  create_info.enable_validation = false;
#else
  create_info.enable_validation = true;
#endif

  std::uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  if (create_info.enable_validation) { extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }

  create_info.pp_extension_names = extensions;

  p_vulkan_instance_ = VulkanInstance::Create(create_info);
  p_vk_instance_ = p_vulkan_instance_->GetVKBInstance().instance;
  VulkanDeviceManager::GetInstance().Init(p_vulkan_instance_);

  // Get Queues
  auto vk_device = VulkanDeviceManager::GetInstance().GetLogicalDevice();

  auto graphics_queue_ret = vk_device.get_queue(vkb::QueueType::graphics);
  ARIA_VKB_CHECK_RESULT_AND_ERROR(graphics_queue_ret, "Failed to get graphics queue");
  graphics_queue_ = graphics_queue_ret.value();

  auto present_queue_ret = vk_device.get_queue(vkb::QueueType::present);
  ARIA_VKB_CHECK_RESULT_AND_ERROR(present_queue_ret, "Failed to get present queue");
  present_queue_ = present_queue_ret.value();

  VulkanGraphicsPipeline::GetInstance().Init();

  CreateCommandPool();
  CreateCommandBuffer();
}

VulkanRendererApi::~VulkanRendererApi() {
  auto vklib = VulkanLib::GetInstance();
  vklib.ptr_vk_destroy_command_pool_(VulkanDeviceManager::GetInstance().GetLogicalDevice(), command_pool_, nullptr);
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

void VulkanRendererApi::CreateCommandPool() {
  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = 0;
  cmd_pool_info.pNext = nullptr;

  auto graphics_queue_index_ret =
      VulkanDeviceManager::GetInstance().GetLogicalDevice().get_queue_index(vkb::QueueType::graphics);
  ARIA_VKB_CHECK_RESULT_AND_ASSERT(graphics_queue_index_ret, "Failed to get graphics queue index")

  cmd_pool_info.queueFamilyIndex = graphics_queue_index_ret.value();

  VkResult result = VulkanLib::GetInstance().ptr_vk_create_command_pool_(
      VulkanDeviceManager::GetInstance().GetLogicalDevice(), &cmd_pool_info, nullptr, &command_pool_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create command pool")
}

void VulkanRendererApi::CreateCommandBuffer() {
  command_buffers_ = std::vector<VkCommandBuffer>(VulkanGraphicsPipeline::GetInstance().GetFrameBuffers().size());

  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = command_pool_;
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = (uint32_t) command_buffers_.size();

  VkResult result = VulkanLib::GetInstance().ptr_vk_allocate_command_buffers_(
      VulkanDeviceManager::GetInstance().GetLogicalDevice(), &buffer_alloc_info, command_buffers_.data());
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to allocate command buffers")
}
}// namespace aria

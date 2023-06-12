#include "ariapch.h"
#include "Platform/Vulkan/VulkanRendererApi.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "Platform/Vulkan/VulkanDeviceManager.h"
#include "Platform/Vulkan/VulkanError.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanLib.h"
#include "VkBootstrap.h"
#include "VulkanDebugMessenger.h"
#include "VulkanError.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanRendererApi.h"
#include <stdint.h>
#include <vector>

namespace aria {

VulkanRendererApi::~VulkanRendererApi() {
  auto vklib = VulkanLib::GetInstance();
  vklib.ptr_vk_destroy_command_pool(VulkanDeviceManager::GetInstance().GetLogicalDevice(), command_pool_, nullptr);

  for (auto semaphore : available_semaphores_) {
    vklib.ptr_vk_destroy_semaphore(VulkanDeviceManager::GetInstance().GetLogicalDevice(), semaphore, nullptr);
  }
  for (auto semaphore : finished_semaphore_) {
    vklib.ptr_vk_destroy_semaphore(VulkanDeviceManager::GetInstance().GetLogicalDevice(), semaphore, nullptr);
  }

  for (auto fence : in_flight_fences_) {
    vklib.ptr_vk_destroy_fence(VulkanDeviceManager::GetInstance().GetLogicalDevice(), fence, nullptr);
  }

  for (auto fence : image_in_flight_) {
    vklib.ptr_vk_destroy_fence(VulkanDeviceManager::GetInstance().GetLogicalDevice(), fence, nullptr);
  }
}

void VulkanRendererApi::Init() {
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

  GetQueues();

  VulkanGraphicsPipeline::GetInstance().Init();

  CreateCommandPool();
  CreateCommandBuffer();
  CreateSyncObjects();
}

void VulkanRendererApi::Clear() { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::SetClearColor(const glm::vec4 color) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::DrawIndexed(const Ref<VertexArray> &vertex_array) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererApi::CreateCommandModule() {

  for (size_t i = 0; i < command_buffers_.size(); i++) {
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pInheritanceInfo = nullptr;
    VkResult result = VulkanLib::GetInstance().ptr_vk_begin_command_buffer(command_buffers_[i], &begin_info);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to begin recording command buffer")

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = VulkanGraphicsPipeline::GetInstance().GetRenderPass()->GetRenderPass();
    render_pass_info.framebuffer =
        VulkanGraphicsPipeline::GetInstance().GetFrameBuffers()[i];//TODO: Need to grab actual index
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = VulkanDeviceManager::GetInstance().GetSwapChain().extent;

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    VulkanLib::GetInstance().ptr_vk_cmd_begin_render_pass(command_buffers_[i], &render_pass_info,
                                                          VK_SUBPASS_CONTENTS_INLINE);
    VulkanLib::GetInstance().ptr_vk_cmd_bind_pipeline(command_buffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                      VulkanGraphicsPipeline::GetInstance().GetGraphicsPipeline());

    vkb::Swapchain swapchain = VulkanDeviceManager::GetInstance().GetSwapChain();
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapchain.extent.width;
    viewport.height = (float) swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VulkanLib::GetInstance().ptr_vk_cmd_set_viewport(command_buffers_[i], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.extent;

    VulkanLib::GetInstance().ptr_vk_cmd_set_scissor(command_buffers_[i], 0, 1, &scissor);

    VulkanLib::GetInstance().ptr_vk_cmd_draw(command_buffers_[i], 3, 1, 0, 0);

    VulkanLib::GetInstance().ptr_vk_cmd_end_render_pass(command_buffers_[i]);

    if (command_buffers_[i] != nullptr) {
      VkResult result = VulkanLib::GetInstance().ptr_vk_end_command_buffer(command_buffers_[i]);
      ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to end recording command buffer")
    }
  }
}

void VulkanRendererApi::BeginRecording() {
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pInheritanceInfo = nullptr;
  VkResult result = VulkanLib::GetInstance().ptr_vk_begin_command_buffer(command_buffers_[0], &begin_info);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to begin recording command buffer")
}

void VulkanRendererApi::EndRecording() {
  if (command_buffers_[0] != nullptr) {
    VkResult result = VulkanLib::GetInstance().ptr_vk_end_command_buffer(command_buffers_[0]);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to end recording command buffer")
  }
}

void VulkanRendererApi::CmdBeginRenderPass() {

  VkRenderPassBeginInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = VulkanGraphicsPipeline::GetInstance().GetRenderPass()->GetRenderPass();
  render_pass_info.framebuffer =
      VulkanGraphicsPipeline::GetInstance().GetFrameBuffers()[0];//TODO: Need to grab actual index
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = VulkanDeviceManager::GetInstance().GetSwapChain().extent;

  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;

  VulkanLib::GetInstance().ptr_vk_cmd_begin_render_pass(command_buffers_[0], &render_pass_info,
                                                        VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRendererApi::CmdEndRenderPass() { VulkanLib::GetInstance().ptr_vk_cmd_end_render_pass(command_buffers_[0]); }

void VulkanRendererApi::CmdBindToGraphicsPipeline() {
  VulkanLib::GetInstance().ptr_vk_cmd_bind_pipeline(command_buffers_[0], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                    VulkanGraphicsPipeline::GetInstance().GetGraphicsPipeline());
}

void VulkanRendererApi::CmdSetViewport() {
  vkb::Swapchain swapchain = VulkanDeviceManager::GetInstance().GetSwapChain();
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) swapchain.extent.width;
  viewport.height = (float) swapchain.extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VulkanLib::GetInstance().ptr_vk_cmd_set_viewport(command_buffers_[0], 0, 1, &viewport);
}

void VulkanRendererApi::CmdSetScissor() {
  vkb::Swapchain swapchain = VulkanDeviceManager::GetInstance().GetSwapChain();
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchain.extent;

  VulkanLib::GetInstance().ptr_vk_cmd_set_scissor(command_buffers_[0], 0, 1, &scissor);
}

void VulkanRendererApi::CmdDraw() { VulkanLib::GetInstance().ptr_vk_cmd_draw(command_buffers_[0], 3, 1, 0, 0); }

void VulkanRendererApi::DrawFrame() {
  auto vklib = VulkanLib::GetInstance();
  vklib.ptr_vk_wait_for_fences(VulkanDeviceManager::GetInstance().GetLogicalDevice(), 1,
                               &in_flight_fences_[current_frame_idx], true, UINT64_MAX);

  uint32_t image_index = 0;
  {
    VkResult result = vklib.ptr_vk_acquire_next_image_khr(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
                                                          VulkanDeviceManager::GetInstance().GetSwapChain(), UINT64_MAX,
                                                          available_semaphores_[0], VK_NULL_HANDLE, &image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      VulkanDeviceManager::GetInstance().RegenerateSwapchain();
      return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      ARIA_CORE_ERROR("Failed to acquire swap chain image!");
      return;
    }
  }

  if (image_in_flight_[image_index] != VK_NULL_HANDLE) {
    vklib.ptr_vk_wait_for_fences(VulkanDeviceManager::GetInstance().GetLogicalDevice(), 1,
                                 &image_in_flight_[image_index], true, UINT64_MAX);
  }
  image_in_flight_[image_index] = in_flight_fences_[current_frame_idx];

  // CreateCommandModule();

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {available_semaphores_[current_frame_idx]};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffers_[image_index];

  VkSemaphore signal_semaphores[] = {finished_semaphore_[current_frame_idx]};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  vklib.ptr_vk_reset_fences(VulkanDeviceManager::GetInstance().GetLogicalDevice(), 1,
                            &in_flight_fences_[current_frame_idx]);

  {
    VkResult result = vklib.ptr_vk_queue_submit(graphics_queue_, 1, &submit_info, in_flight_fences_[current_frame_idx]);
    ARIA_VK_CHECK_RESULT_AND_ASSERT(result, "Failed to submit command buffer")
  }

  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swapchains[] = {VulkanDeviceManager::GetInstance().GetSwapChain().swapchain};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swapchains;

  present_info.pImageIndices = &image_index;

  {
    VkResult result = vklib.ptr_vk_queue_present_khr(present_queue_, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
      VulkanDeviceManager::GetInstance().RegenerateSwapchain();
      return;
    } else if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to present swap chain image!");
      return;
    }
  }

  current_frame_idx = (current_frame_idx + 1) % max_frames_in_flight_;
}

void VulkanRendererApi::AddToPipeline(VkShaderModule &shader_module, ShaderType type) {
  VulkanGraphicsPipeline::GetInstance().AddToShaderStages(shader_module, type);
}

void VulkanRendererApi::CreateCommandPool() {
  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  cmd_pool_info.pNext = nullptr;

  auto graphics_queue_index_ret =
      VulkanDeviceManager::GetInstance().GetLogicalDevice().get_queue_index(vkb::QueueType::graphics);
  ARIA_VKB_CHECK_RESULT_AND_ASSERT(graphics_queue_index_ret, "Failed to get graphics queue index")

  cmd_pool_info.queueFamilyIndex = graphics_queue_index_ret.value();

  VkResult result = VulkanLib::GetInstance().ptr_vk_create_command_pool(
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

  VkResult result = VulkanLib::GetInstance().ptr_vk_allocate_command_buffers(
      VulkanDeviceManager::GetInstance().GetLogicalDevice(), &buffer_alloc_info, command_buffers_.data());
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to allocate command buffers")
}

void VulkanRendererApi::CreateSyncObjects() {
  available_semaphores_.resize(max_frames_in_flight_);
  finished_semaphore_.resize(max_frames_in_flight_);
  in_flight_fences_.resize(max_frames_in_flight_);
  image_in_flight_.resize(VulkanDeviceManager::GetInstance().GetSwapChain().image_count);

  VkSemaphoreCreateInfo semaphore_info;
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_info.pNext = nullptr;
  semaphore_info.flags = 0;

  VkFenceCreateInfo fence_info;
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.pNext = nullptr;
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (int i = 0; i < max_frames_in_flight_; ++i) {
    VkResult result = VulkanLib::GetInstance().ptr_vk_create_semaphore(
        VulkanDeviceManager::GetInstance().GetLogicalDevice(), &semaphore_info, nullptr, &available_semaphores_[i]);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create semaphore")
    result = VulkanLib::GetInstance().ptr_vk_create_semaphore(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
                                                              &semaphore_info, nullptr, &finished_semaphore_[i]);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create semaphore")

    result = VulkanLib::GetInstance().ptr_vk_create_fence(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
                                                          &fence_info, nullptr, &in_flight_fences_[i]);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create fence")
  }
}

void VulkanRendererApi::GetQueues() {
  // Get Queues
  auto vk_device = VulkanDeviceManager::GetInstance().GetLogicalDevice();

  auto graphics_queue_ret = vk_device.get_queue(vkb::QueueType::graphics);
  ARIA_VKB_CHECK_RESULT_AND_ERROR(graphics_queue_ret, "Failed to get graphics queue");
  graphics_queue_ = graphics_queue_ret.value();

  auto present_queue_ret = vk_device.get_queue(vkb::QueueType::present);
  ARIA_VKB_CHECK_RESULT_AND_ERROR(present_queue_ret, "Failed to get present queue");
  present_queue_ = present_queue_ret.value();
}
}// namespace aria

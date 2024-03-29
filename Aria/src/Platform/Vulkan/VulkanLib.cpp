
#include "VulkanLib.h"
#include "Aria/Core/Log.h"

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

namespace aria {

VulkanLib::VulkanLib() : library_(nullptr) { Init(); }

VulkanLib::~VulkanLib() { Cleanup(); }

bool VulkanLib::LoadVulkanLib() {
#if defined(__linux__)
  library_ = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
  if (!library_) library_ = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#elif defined(__APPLE__)
  library_ = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
  if (!library_) library_ = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
#elif defined(_WIN32)
  library_ = LoadLibrary(TEXT("vulkan-1.dll"));
#else
  ARIA_CORE_ASSERT(false, "Unsupported platform");
#endif

  if (!library_) {
#if ARIA_DEBUG
    DWORD error_code = GetLastError();
    LPSTR message_buffer = nullptr;
    DWORD message_size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
        error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&message_buffer), 0, nullptr);

    std::string error_message(message_buffer, message_size);

    LocalFree(message_buffer);

    ARIA_CORE_ASSERT(false, "Failed to load vulkan runtime library - " + error_message);
    return false;
#else
    ARIA_CORE_ASSERT(false, "Failed to load vulkan runtime library")
    return false;
#endif
  }
  ARIA_CORE_INFO("Loaded vulkan runtime library")
  return true;
}

template<typename T>
void VulkanLib::LoadFunc(T &func_dest, const char *func_name) {
#if defined(__linux__) || defined(__APPLE__)
  func_dest = reinterpret_cast<T>(dlsym(library_, func_name));
#elif defined(_WIN32)
  func_dest = reinterpret_cast<T>(GetProcAddress(library_, func_name));
#endif
  if (!func_dest) { ARIA_CORE_ASSERT(false, "Failed to load vulkan function - {0}", func_name) }
}

PFN_vkVoidFunction VulkanLib::LoadFunc(const char *func_name, void *user_data) {
  PFN_vkVoidFunction func_dest;
#if defined(__linux__) || defined(__APPLE__)
  func_dest = reinterpret_cast<PFN_vkVoidFunction>(dlsym(library_, func_name));
#elif defined(_WIN32)
  func_dest = reinterpret_cast<PFN_vkVoidFunction>(GetProcAddress(library_, func_name));
#endif
  if (!func_dest) { ARIA_CORE_ASSERT(false, "Failed to load vulkan function - {0}", func_name) }
  return func_dest;
}

/// Initialize functions that don't require a vkInstance
void VulkanLib::Init() {
  if (!LoadVulkanLib()) return;
  LoadFunc(ptr_vk_get_instance_proc_addr, "vkGetInstanceProcAddr");
  LoadFunc(ptr_vk_get_device_proc_addr, "vkGetDeviceProcAddr");

  ptr_vk_create_instance =
      reinterpret_cast<PFN_vkCreateInstance>(ptr_vk_get_instance_proc_addr(VK_NULL_HANDLE, "vkCreateInstance"));
}

void VulkanLib::InitInstanceFunctions(VkInstance instance) {
  ptr_vk_create_render_pass =
      reinterpret_cast<PFN_vkCreateRenderPass>(ptr_vk_get_instance_proc_addr(instance, "vkCreateRenderPass"));
  ptr_vk_destroy_render_pass =
      reinterpret_cast<PFN_vkDestroyRenderPass>(ptr_vk_get_instance_proc_addr(instance, "vkDestroyRenderPass"));
}

void VulkanLib::InitDeviceFunctions(VkDevice device) {
  ptr_vk_create_pipeline_layout =
      reinterpret_cast<PFN_vkCreatePipelineLayout>(ptr_vk_get_device_proc_addr(device, "vkCreatePipelineLayout"));
  ptr_vk_destroy_pipeline_layout =
      reinterpret_cast<PFN_vkDestroyPipelineLayout>(ptr_vk_get_device_proc_addr(device, "vkDestroyPipelineLayout"));
  ptr_vk_create_render_pass =
      reinterpret_cast<PFN_vkCreateRenderPass>(ptr_vk_get_device_proc_addr(device, "vkCreateRenderPass"));
  ptr_vk_destroy_render_pass =
      reinterpret_cast<PFN_vkDestroyRenderPass>(ptr_vk_get_device_proc_addr(device, "vkDestroyRenderPass"));
  ptr_vk_create_graphics_pipelines =
      reinterpret_cast<PFN_vkCreateGraphicsPipelines>(ptr_vk_get_device_proc_addr(device, "vkCreateGraphicsPipelines"));
  ptr_vk_destroy_pipeline =
      reinterpret_cast<PFN_vkDestroyPipeline>(ptr_vk_get_device_proc_addr(device, "vkDestroyPipeline"));
  ptr_vk_create_framebuffer =
      reinterpret_cast<PFN_vkCreateFramebuffer>(ptr_vk_get_device_proc_addr(device, "vkCreateFramebuffer"));
  ptr_vk_destroy_framebuffer =
      reinterpret_cast<PFN_vkDestroyFramebuffer>(ptr_vk_get_device_proc_addr(device, "vkDestroyFramebuffer"));
  ptr_vk_create_command_pool =
      reinterpret_cast<PFN_vkCreateCommandPool>(ptr_vk_get_device_proc_addr(device, "vkCreateCommandPool"));
  ptr_vk_destroy_command_pool =
      reinterpret_cast<PFN_vkDestroyCommandPool>(ptr_vk_get_device_proc_addr(device, "vkDestroyCommandPool"));
  ptr_vk_allocate_command_buffers =
      reinterpret_cast<PFN_vkAllocateCommandBuffers>(ptr_vk_get_device_proc_addr(device, "vkAllocateCommandBuffers"));
  ptr_vk_create_shader_module =
      reinterpret_cast<PFN_vkCreateShaderModule>(ptr_vk_get_device_proc_addr(device, "vkCreateShaderModule"));
  ptr_vk_destroy_shader_module =
      reinterpret_cast<PFN_vkDestroyShaderModule>(ptr_vk_get_device_proc_addr(device, "vkDestroyShaderModule"));
  ptr_vk_create_semaphore =
      reinterpret_cast<PFN_vkCreateSemaphore>(ptr_vk_get_device_proc_addr(device, "vkCreateSemaphore"));
  ptr_vk_destroy_semaphore =
      reinterpret_cast<PFN_vkDestroySemaphore>(ptr_vk_get_device_proc_addr(device, "vkDestroySemaphore"));
  ptr_vk_create_fence = reinterpret_cast<PFN_vkCreateFence>(ptr_vk_get_device_proc_addr(device, "vkCreateFence"));
  ptr_vk_destroy_fence = reinterpret_cast<PFN_vkDestroyFence>(ptr_vk_get_device_proc_addr(device, "vkDestroyFence"));
  ptr_vk_create_pipeline_cache =
      reinterpret_cast<PFN_vkCreatePipelineCache>(ptr_vk_get_device_proc_addr(device, "vkCreatePipelineCache"));
  ptr_vk_destroy_pipeline_cache =
      reinterpret_cast<PFN_vkDestroyPipelineCache>(ptr_vk_get_device_proc_addr(device, "vkDestroyPipelineCache"));
  ptr_vk_wait_for_fences =
      reinterpret_cast<PFN_vkWaitForFences>(ptr_vk_get_device_proc_addr(device, "vkWaitForFences"));
  ptr_vk_reset_fences = reinterpret_cast<PFN_vkResetFences>(ptr_vk_get_device_proc_addr(device, "vkResetFences"));
  ptr_vk_acquire_next_image_khr =
      reinterpret_cast<PFN_vkAcquireNextImageKHR>(ptr_vk_get_device_proc_addr(device, "vkAcquireNextImageKHR"));
  ptr_vk_device_wait_idle =
      reinterpret_cast<PFN_vkDeviceWaitIdle>(ptr_vk_get_device_proc_addr(device, "vkDeviceWaitIdle"));

  ptr_vk_begin_command_buffer =
      reinterpret_cast<PFN_vkBeginCommandBuffer>(ptr_vk_get_device_proc_addr(device, "vkBeginCommandBuffer"));
  ptr_vk_reset_command_buffer =
      reinterpret_cast<PFN_vkResetCommandBuffer>(ptr_vk_get_device_proc_addr(device, "vkResetCommandBuffer"));
  ptr_vk_end_command_buffer =
      reinterpret_cast<PFN_vkEndCommandBuffer>(ptr_vk_get_device_proc_addr(device, "vkEndCommandBuffer"));
  ptr_vk_cmd_begin_render_pass =
      reinterpret_cast<PFN_vkCmdBeginRenderPass>(ptr_vk_get_device_proc_addr(device, "vkCmdBeginRenderPass"));
  ptr_vk_cmd_end_render_pass =
      reinterpret_cast<PFN_vkCmdEndRenderPass>(ptr_vk_get_device_proc_addr(device, "vkCmdEndRenderPass"));
  ptr_vk_cmd_bind_pipeline =
      reinterpret_cast<PFN_vkCmdBindPipeline>(ptr_vk_get_device_proc_addr(device, "vkCmdBindPipeline"));
  ptr_vk_cmd_set_viewport =
      reinterpret_cast<PFN_vkCmdSetViewport>(ptr_vk_get_device_proc_addr(device, "vkCmdSetViewport"));
  ptr_vk_cmd_set_scissor =
      reinterpret_cast<PFN_vkCmdSetScissor>(ptr_vk_get_device_proc_addr(device, "vkCmdSetScissor"));
  ptr_vk_cmd_draw = reinterpret_cast<PFN_vkCmdDraw>(ptr_vk_get_device_proc_addr(device, "vkCmdDraw"));
  ptr_vk_queue_submit = reinterpret_cast<PFN_vkQueueSubmit>(ptr_vk_get_device_proc_addr(device, "vkQueueSubmit"));
  ptr_vk_queue_present_khr =
      reinterpret_cast<PFN_vkQueuePresentKHR>(ptr_vk_get_device_proc_addr(device, "vkQueuePresentKHR"));
}

void VulkanLib::Cleanup() {
#ifdef _WIN32
  FreeLibrary(library_);
#endif
#if defined(__linux__) || defined(__APPLE__)
  dlclose(library_);
#endif
  library_ = nullptr;
}
}// namespace aria

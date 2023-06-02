
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

/// Initialize functions that don't require a vkInstance
void VulkanLib::Init() {
  if (!LoadVulkanLib()) return;
  LoadFunc(ptr_vk_get_instance_proc_addr_, "vkGetInstanceProcAddr");
  LoadFunc(ptr_vk_get_device_proc_addr_, "vkGetDeviceProcAddr");

  ptr_vk_create_instance_ =
      reinterpret_cast<PFN_vkCreateInstance>(ptr_vk_get_instance_proc_addr_(VK_NULL_HANDLE, "vkCreateInstance"));
}

void VulkanLib::InitInstanceFunctions(VkInstance instance) {
  ptr_vk_create_render_pass_ =
      reinterpret_cast<PFN_vkCreateRenderPass>(ptr_vk_get_instance_proc_addr_(instance, "vkCreateRenderPass"));
  ptr_vk_destroy_render_pass_ =
      reinterpret_cast<PFN_vkDestroyRenderPass>(ptr_vk_get_instance_proc_addr_(instance, "vkDestroyRenderPass"));
}

void VulkanLib::InitDeviceFunctions(VkDevice device) {
  ptr_vk_create_pipeline_layout_ =
      reinterpret_cast<PFN_vkCreatePipelineLayout>(ptr_vk_get_device_proc_addr_(device, "vkCreatePipelineLayout"));
  ptr_vk_destroy_pipeline_layout_ =
      reinterpret_cast<PFN_vkDestroyPipelineLayout>(ptr_vk_get_device_proc_addr_(device, "vkDestroyPipelineLayout"));
  ptr_vk_create_render_pass_ =
      reinterpret_cast<PFN_vkCreateRenderPass>(ptr_vk_get_device_proc_addr_(device, "vkCreateRenderPass"));
  ptr_vk_destroy_render_pass_ =
      reinterpret_cast<PFN_vkDestroyRenderPass>(ptr_vk_get_device_proc_addr_(device, "vkDestroyRenderPass"));
  ptr_vk_create_graphics_pipelines_ = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(
      ptr_vk_get_device_proc_addr_(device, "vkCreateGraphicsPipelines"));
  ptr_vk_destroy_pipeline_ =
      reinterpret_cast<PFN_vkDestroyPipeline>(ptr_vk_get_device_proc_addr_(device, "vkDestroyPipeline"));
  ptr_vk_create_framebuffer_ =
      reinterpret_cast<PFN_vkCreateFramebuffer>(ptr_vk_get_device_proc_addr_(device, "vkCreateFramebuffer"));
  ptr_vk_destroy_framebuffer_ =
      reinterpret_cast<PFN_vkDestroyFramebuffer>(ptr_vk_get_device_proc_addr_(device, "vkDestroyFramebuffer"));
  ptr_vk_create_command_pool_ =
      reinterpret_cast<PFN_vkCreateCommandPool>(ptr_vk_get_device_proc_addr_(device, "vkCreateCommandPool"));
  ptr_vk_destroy_command_pool_ =
      reinterpret_cast<PFN_vkDestroyCommandPool>(ptr_vk_get_device_proc_addr_(device, "vkDestroyCommandPool"));
  ptr_vk_allocate_command_buffers_ =
      reinterpret_cast<PFN_vkAllocateCommandBuffers>(ptr_vk_get_device_proc_addr_(device, "vkAllocateCommandBuffers"));
  ptr_vk_create_shader_module_ =
      reinterpret_cast<PFN_vkCreateShaderModule>(ptr_vk_get_device_proc_addr_(device, "vkCreateShaderModule"));
  ptr_vk_destroy_shader_module_ =
      reinterpret_cast<PFN_vkDestroyShaderModule>(ptr_vk_get_device_proc_addr_(device, "vkDestroyShaderModule"));
  ptr_vk_create_semaphore_ =
      reinterpret_cast<PFN_vkCreateSemaphore>(ptr_vk_get_device_proc_addr_(device, "vkCreateSemaphore"));
  ptr_vk_destroy_semaphore_ =
      reinterpret_cast<PFN_vkDestroySemaphore>(ptr_vk_get_device_proc_addr_(device, "vkDestroySemaphore"));
  ptr_vk_create_fence_ = reinterpret_cast<PFN_vkCreateFence>(ptr_vk_get_device_proc_addr_(device, "vkCreateFence"));
  ptr_vk_destroy_fence_ = reinterpret_cast<PFN_vkDestroyFence>(ptr_vk_get_device_proc_addr_(device, "vkDestroyFence"));
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
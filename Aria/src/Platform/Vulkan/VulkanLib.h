#pragma once

#include <vulkan/vulkan.h>

namespace aria {

class VulkanLib {
 public:
  VulkanLib();
  ~VulkanLib();

  static VulkanLib &GetInstance() {
    static VulkanLib instance_;
    return instance_;
  }

  bool LoadVulkanLib();
  void InitInstanceFunctions(VkInstance instance);
  void InitDeviceFunctions(VkDevice device);
  void Cleanup();

  template<typename T>
  void LoadFunc(T &func_dest, const char *func_name);

  // Non vkInstance function pointers
  PFN_vkGetInstanceProcAddr ptr_vk_get_instance_proc_addr_ = nullptr;
  PFN_vkGetDeviceProcAddr ptr_vk_get_device_proc_addr_ = nullptr;

  // vkInstance function pointers
  PFN_vkCreateInstance ptr_vk_create_instance_ = nullptr;

  // vkDevice function pointers
  PFN_vkCreatePipelineLayout ptr_vk_create_pipeline_layout_ = nullptr;
  PFN_vkDestroyPipelineLayout ptr_vk_destroy_pipeline_layout_ = nullptr;
  PFN_vkCreateRenderPass ptr_vk_create_render_pass_ = nullptr;
  PFN_vkDestroyRenderPass ptr_vk_destroy_render_pass_ = nullptr;
  PFN_vkCreateGraphicsPipelines ptr_vk_create_graphics_pipelines_ = nullptr;
  PFN_vkDestroyPipeline ptr_vk_destroy_pipeline_ = nullptr;
  PFN_vkCreateFramebuffer ptr_vk_create_framebuffer_ = nullptr;
  PFN_vkDestroyFramebuffer ptr_vk_destroy_framebuffer_ = nullptr;
  PFN_vkCreateCommandPool ptr_vk_create_command_pool_ = nullptr;
  PFN_vkDestroyCommandPool ptr_vk_destroy_command_pool_ = nullptr;
  PFN_vkAllocateCommandBuffers ptr_vk_allocate_command_buffers_ = nullptr;
  PFN_vkCreateShaderModule ptr_vk_create_shader_module_ = nullptr;
  PFN_vkDestroyShaderModule ptr_vk_destroy_shader_module_ = nullptr;
  PFN_vkCreateSemaphore ptr_vk_create_semaphore_ = nullptr;
  PFN_vkDestroySemaphore ptr_vk_destroy_semaphore_ = nullptr;
  PFN_vkCreateFence ptr_vk_create_fence_ = nullptr;
  PFN_vkDestroyFence ptr_vk_destroy_fence_ = nullptr;

 private:
  void Init();

#ifdef _WIN32
  HMODULE library_;
#elif defined(__linux__) || defined(__APPLE__)
  void *library_;
#endif
};

}// namespace aria
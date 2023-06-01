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
  void Cleanup();

  template<typename T>
  void LoadFunc(T &func_dest, const char *func_name);

 private:
  void Init();
  
#ifdef _WIN32
  HMODULE library_;
#elif defined(__linux__) || defined(__APPLE__)
  void *library_;
#endif

  // Non vkInstance function pointers
  PFN_vkGetInstanceProcAddr ptr_vk_get_instance_proc_addr_ = nullptr;

  // vkInstance function pointers
  PFN_vkCreateInstance ptr_vk_create_instance_ = nullptr;
  PFN_vkCreateRenderPass ptr_vk_create_render_pass_ = nullptr;
  PFN_vkDestroyRenderPass ptr_vk_destroy_render_pass_ = nullptr;
};

}// namespace aria
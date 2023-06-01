#pragma once
#include <vulkan/vulkan.h>

namespace aria {
template<typename FuncType>
class VulkanFunction {
 public:
  static FuncType Load(const char *name);
  ~VulkanFunction();

 private:
  static void *VulkanLibrary();
};

// Declare function pointers using the template for Vulkan API functions you plan to use
using PFN_vkCreateInstance = VkResult(VKAPI_PTR *)(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance);
using vkCreateInstancePtr = VulkanFunction<PFN_vkCreateInstance>;
}// namespace aria
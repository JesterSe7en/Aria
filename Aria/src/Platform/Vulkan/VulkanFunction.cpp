#include "ariapch.h"
#include "VulkanFunction.h"
#include "Aria/Core/Log.h"
#include <vulkan/vulkan.h>

namespace aria {

template<typename FuncType>
FuncType VulkanFunction<FuncType>::Load(const char *name) {
#ifdef _WIN32
  return reinterpret_cast<FuncType>(GetProcAddress(VulkanLibrary(), name));
#else
  return reinterpret_cast<FuncType>(dlsym(VulkanLibrary(), name));
#endif
}

template<typename FuncType>
VulkanFunction<FuncType>::~VulkanFunction() {
  // Clean up and unload the Vulkan library
#ifdef _WIN32
  FreeLibrary(static_cast<HMODULE>(vkCreateInstancePtr::VulkanLibrary()));
#else
  dlclose(vkCreateInstancePtr::VulkanLibrary());
#endif
}

template<typename FuncType>
void *VulkanFunction<FuncType>::VulkanLibrary() {
#ifdef _WIN32
  static HMODULE library = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(__APPLE__)
      static void *library = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
      if (!library) library = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
#elif defined(__linux__)
      static void *library = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
      if (!library) library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#else
      ARIA_CORE_ASSERT(false, "Unsupported platform")
#endif
  if (library == nullptr) {
    ARIA_CORE_ASSERT(false, "Failed to load Vulkan runtime library")
  }
  return library;
}

//
//template<typename FuncType>
//class VulkanFunction {
// public:
//  static FuncType Load(const char *name) {
//#ifdef _WIN32
//    return reinterpret_cast<FuncType>(GetProcAddress(VulkanLibrary(), name));
//#else
//    return reinterpret_cast<FuncType>(dlsym(VulkanLibrary(), name));
//#endif
//  }
//
//  ~VulkanFunction() {
//    // Clean up and unload the Vulkan library
//#ifdef _WIN32
//  FreeLibrary(static_cast<HMODULE>(vkCreateInstancePtr::VulkanLibrary()));
//#else
//  dlclose(vkCreateInstancePtr::VulkanLibrary());
//#endif
//  }
//
// private:
//  static void *VulkanLibrary() {
//#ifdef _WIN32
//    static HMODULE library = LoadLibrary(TEXT("vulkan-1.dll"));
//#elif defined(__APPLE__)
//    static void *library = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
//    if (!library) library = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
//#elif defined(__linux__)
//    static void *library = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
//    if (!library) library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
//#else
//    ARIA_CORE_ASSERT(false, "Unsupported platform")
//#endif
//    if (library == nullptr) {
//      ARIA_CORE_ASSERT(false, "Failed to load Vulkan runtime library")
//    }
//    return library;
//  }
//};
//
//// Declare function pointers using the template for Vulkan API functions you plan to use
//using PFN_vkCreateInstance = VkResult(VKAPI_PTR *)(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance);
//using vkCreateInstancePtr = VulkanFunction<PFN_vkCreateInstance>;

//// Example usage
//int main() {
//  // Load the vkCreateInstance function pointer
//  vkCreateInstancePtr::Load("vkCreateInstance");
//
//  // Call the vkCreateInstance function
//  VkInstanceCreateInfo createInfo{};
//  VkInstance instance;
//  VkResult result = vkCreateInstancePtr::Load("vkCreateInstance")(&createInfo, nullptr, &instance);
//  if (result != VK_SUCCESS) {
//    ARIA_CORE_ERROR("Failed to create Vulkan instance")
//    // Handle the error appropriately
//  }
//
//  // Use the Vulkan instance...
//
//  
//
//  return 0;
//}
}// namespace aria

#pragma once
#include <iostream>
#include <vulkan/vulkan.h>

template<typename FuncType>
class VulkanFunction {
public:
    static FuncType Load(const char* name);

private:
    static void* VulkanLibrary();
};

// Declare function pointers using the template for Vulkan API functions you plan to use
using PFN_vkCreateInstance = VkResult(VKAPI_PTR*)(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
using vkCreateInstancePtr = VulkanFunction<PFN_vkCreateInstance>;

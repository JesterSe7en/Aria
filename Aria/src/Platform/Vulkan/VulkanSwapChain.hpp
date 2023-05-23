#pragma once

#include "vulkan/vulkan_core.h"
#include "VulkanInstance.hpp"
#include "VulkanDeviceManager.hpp"

namespace aria {

class VulkanSwapChain {
 public:

  struct VulkanSwapChainDetails {
    VkFormat swap_chain_format;
    VkExtent2D swap_chain_extend_2d;
    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
  };

  ~VulkanSwapChain();

  VulkanSwapChain(const VulkanSwapChain &) = delete;
  VulkanSwapChain &operator=(const VulkanSwapChain &) = delete;

  static Ref<VulkanSwapChain> Create();

  VulkanSwapChainDetails GetSwapChainDetails() { return vulkan_swap_chain_details_; }
  VkSwapchainKHR GetSwapChain() { return p_vk_swapchain_khr_; }


 private:
  class PhysicalDeviceSurfaceSwapChainDetails {
   public:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  VkSwapchainKHR p_vk_swapchain_khr_ = VK_NULL_HANDLE;
  VkSurfaceKHR p_vk_surface_khr_ = VK_NULL_HANDLE;
  Ref<VulkanInstance> p_vk_instance_;
  VkPhysicalDevice p_vk_physical_device_;
  VkDevice p_vk_device_;
  VulkanSwapChainDetails vulkan_swap_chain_details_;

  VulkanSwapChain();
  void CreatePresentationSurface();
  void CreateSwapChain();
  void CreateImageViews();

  PhysicalDeviceSurfaceSwapChainDetails QuerySwapChainSupport();

  // ----- For Swap Chain ------
  static VkSurfaceFormatKHR GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
  static VkPresentModeKHR GetPresentMode(const std::vector<VkPresentModeKHR> &modes);
  static VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};

} // aria



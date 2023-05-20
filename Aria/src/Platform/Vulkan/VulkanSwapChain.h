#pragma once

#include "vulkan/vulkan_core.h"
#include "VulkanDeviceManager.h"

namespace aria {

class VulkanSwapChain {
 public:

  struct VulkanSwapChainDetails {
    VkFormat swap_chain_format;
    VkExtent2D swap_chain_extend_2d;
    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
    std::vector<VkFramebuffer> swap_chain_frame_buffers;
  };

  static VulkanSwapChain &GetInstance() {
    static VulkanSwapChain instance;
    return instance;
  }

  VulkanSwapChainDetails GetSwapChainDetails() { return vulkan_swap_chain_details_; }

  ~VulkanSwapChain();
  VulkanSwapChain(const VulkanSwapChain &) = delete;
  VulkanSwapChain &operator=(const VulkanSwapChain &) = delete;

 private:
  VulkanSwapChain();

  void CreateSwapChain();
  void CreateImageViews();

  VulkanSwapChainDetails vulkan_swap_chain_details_;

  // ----- For Swap Chain ------
  static VkSurfaceFormatKHR GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
  static VkPresentModeKHR GetPresentMode(const std::vector<VkPresentModeKHR> &modes);
  static VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
};

} // aria



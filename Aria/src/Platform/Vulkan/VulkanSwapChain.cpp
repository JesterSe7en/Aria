#include "VulkanSwapChain.h"
#include "Aria/Core/Log.h"
#include "VulkanRendererApi.h"
#include "vulkan/vk_enum_string_helper.h"
#include "GLFW/glfw3.h"
#include "Aria/Core/Application.h"

#include <limits>

namespace aria {

VulkanSwapChain::~VulkanSwapChain() {
  for (auto view : vulkan_swap_chain_details_.swap_chain_image_views) {
    vkDestroyImageView(VulkanDeviceManager::GetLogicalDevice(), view, nullptr);
  }
  vkDestroySwapchainKHR(VulkanDeviceManager::GetLogicalDevice(), vk_swapchain_khr_, nullptr);
}

void VulkanSwapChain::Init() {
  CreateSwapChain();
  CreateImageViews();
}

void VulkanSwapChain::CreateSwapChain() {
  VulkanDeviceManager::PhysicalDeviceSurfaceSwapChainDetails
      details = VulkanDeviceManager::GetInstance().GetSwapChainSupportDetails();

  VkSurfaceFormatKHR surface_format = GetSwapSurfaceFormat(details.formats);
  VkPresentModeKHR present_mode = GetPresentMode(details.present_modes);
  VkExtent2D extent = GetSwapExtent(details.capabilities);

  std::uint32_t image_count = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
    image_count = details.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info;
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.pNext = nullptr;
  create_info.flags = 0;
  create_info.surface = VulkanRendererApi::GetVkSurfaceKHR();
  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  VulkanDeviceManager::QueueFamilyIndices indices = VulkanDeviceManager::GetInstance().GetQueueFamilyIndicies();
  std::array<std::uint32_t, 2> queue_family_indices = {indices.graphics_family.value(), indices.present_family.value()};

  if (indices.graphics_family != indices.present_family) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices.data();
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;
    create_info.pQueueFamilyIndices = nullptr;
  }

  create_info.preTransform = details.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  // this is in the event window size has been changed, existing swap chain is invalid
  // as a swap chain is tied to window size
  create_info.oldSwapchain = VK_NULL_HANDLE;

  VkResult
      result = vkCreateSwapchainKHR(VulkanDeviceManager::GetLogicalDevice(), &create_info, nullptr, &vk_swapchain_khr_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create swap chain - {0}", string_VkResult(result))
  }

  vkGetSwapchainImagesKHR(VulkanDeviceManager::GetLogicalDevice(), vk_swapchain_khr_, &image_count, nullptr);
  vulkan_swap_chain_details_.swap_chain_images.resize(image_count);
  vkGetSwapchainImagesKHR(VulkanDeviceManager::GetLogicalDevice(),
                          vk_swapchain_khr_,
                          &image_count,
                          vulkan_swap_chain_details_.swap_chain_images.data());
  vulkan_swap_chain_details_.swap_chain_format = surface_format.format;
  vulkan_swap_chain_details_.swap_chain_extend_2d = extent;
}

void VulkanSwapChain::CreateImageViews() {
  vulkan_swap_chain_details_.swap_chain_image_views.resize(vulkan_swap_chain_details_.swap_chain_images.size());

  for (size_t idx = 0; idx < vulkan_swap_chain_details_.swap_chain_images.size(); idx++) {
    VkImageViewCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.image = vulkan_swap_chain_details_.swap_chain_images[idx];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = vulkan_swap_chain_details_.swap_chain_format;

    create_info.components.r = create_info.components.g = create_info.components.b = create_info.components.a =
        VK_COMPONENT_SWIZZLE_IDENTITY;

    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    VkResult result = vkCreateImageView(VulkanDeviceManager::GetLogicalDevice(),
                                        &create_info,
                                        nullptr,
                                        &vulkan_swap_chain_details_.swap_chain_image_views[idx]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create image views - {0}", string_VkResult(result))
    }
  }
}

// By default, use VK_FORMAT_B8G8R8A8_SRGB and sRGB color space
VkSurfaceFormatKHR VulkanSwapChain::GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) {
  for (const auto &kFormat : formats) {
    if (kFormat.format == VK_FORMAT_B8G8R8A8_SRGB && kFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return kFormat;
    }
  }

  return formats[0];
}

VkPresentModeKHR VulkanSwapChain::GetPresentMode(const std::vector<VkPresentModeKHR> &modes) {
  for (const auto &kMode : modes) {
    if (kMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return kMode;
    }
  }
  // Only guaranteed this mode
  // vulkan spec v1.3 pg 2717
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    auto *glfw_window = (GLFWwindow *) Application::Get().GetWindow().GetNativeWindow();
    int width;
    int height;

    glfwGetFramebufferSize(glfw_window, &width, &height);

    VkExtent2D extent = {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};

    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return extent;
  }
}

} // aria
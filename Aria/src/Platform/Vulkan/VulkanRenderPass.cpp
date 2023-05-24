#include "VulkanRenderPass.hpp"
#include "VulkanError.hpp"

namespace aria {

VulkanRenderPass::~VulkanRenderPass() { vkDestroyRenderPass(vk_device_, vk_render_pass_, nullptr); }

VulkanRenderPass::VulkanRenderPass() {
  // TODO: Need to get access to swapchain and logical device

  VkAttachmentDescription color_attachment;
  VkResult result;

  color_attachment.format = vulkan_swap_chain_.GetSwapChainDetails().swap_chain_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref;
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass;
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_info;
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;

  result = vkCreateRenderPass(vk_device_, &render_pass_info, nullptr, &vk_render_pass_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create render pass");
}

Ref<VulkanRenderPass> VulkanRenderPass::Create() {
  auto instance = new VulkanRenderPass();
  return Ref<VulkanRenderPass>(instance);
}

}// namespace aria

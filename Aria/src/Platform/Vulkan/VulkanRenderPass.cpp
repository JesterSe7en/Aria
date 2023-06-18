#include "VulkanRenderPass.h"
#include "VulkanDeviceManager.h"
#include "VulkanRendererApi.h"
#include "VulkanError.h"

namespace aria {

VulkanRenderPass::~VulkanRenderPass() {
  Ref<VulkanDeviceManager> device_manager = VulkanRendererApi::GetInstance().GetVkDeviceManager();
  vklib_.ptr_vk_destroy_render_pass(device_manager->GetLogicalDevice(), vk_render_pass_, nullptr);
}

VulkanRenderPass::VulkanRenderPass() {
  vklib_ = VulkanLib::GetInstance();

  VkResult result;
  Ref<VulkanDeviceManager> device_manager = VulkanRendererApi::GetInstance().GetVkDeviceManager();
  vkb::Swapchain swapchain = device_manager->GetSwapChain();

  VkAttachmentDescription color_attachment = {};
  color_attachment.format = swapchain.image_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref = {};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_info = {};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  result = vklib_.ptr_vk_create_render_pass(device_manager->GetLogicalDevice(), &render_pass_info, nullptr,
                                            &vk_render_pass_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create render pass")
}

Ref<VulkanRenderPass> VulkanRenderPass::Create() {
  auto instance = new VulkanRenderPass();
  return Ref<VulkanRenderPass>(instance);
}

}// namespace aria

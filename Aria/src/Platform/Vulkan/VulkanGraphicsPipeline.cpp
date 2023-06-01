#include "VulkanSwapChain.h"
#include "ariapch.h"
#include "VulkanGraphicsPipeline.h"
#include "Aria/Core/Log.h"
#include "VulkanRendererApi.h"
#include "vulkan/vk_enum_string_helper.h"

namespace aria {

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
  for (auto buffer : vk_frame_buffers_) {
    vkDestroyFramebuffer(VulkanDeviceManager::GetInstance().GetLogicalDevice(), buffer, nullptr);
  }
  vkDestroyPipeline(VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_graphics_pipeline_, nullptr);
  vkDestroyPipelineLayout(VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_pipeline_layout_, nullptr);
}

void VulkanGraphicsPipeline::Init() {
  //  CreateGraphicsPipeline();
  //  CreateFrameBuffers();
}

//void VulkanGraphicsPipeline::CreateGraphicsPipeline() {
//  //
//
//  // ======================== Shader Create Info ========================
//  // VkPipelineShaderStageCreateInfo vertex_shader_stage_info;
//  // vertex_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//  // vertex_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
//  // // TODO: vertex_shader_stage_info.module = Need to grab vertex shader from ShaderLibrary in VulkanLayer
//  // vertex_shader_stage_info.pName = "main";
//
//  // VkPipelineShaderStageCreateInfo frag_shader_stage_info;
//  // frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//  // frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//  // // TODO: frag_shader_stage_info.module = Need to grab frag shader from ShaderLibrary in VulkanLayer
//  // frag_shader_stage_info.pName = "main";
//
//  // std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages = {vertex_shader_stage_info, frag_shader_stage_info};
//  VulkanSwapChain::VulkanSwapChainDetails swap_chain_details = VulkanSwapChain::GetInstance().GetSwapChainDetails();
//
//  // ======================== Dynamic Create Info ========================
//
//  VkPipelineDynamicStateCreateInfo dynamic_state{};
//  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//  dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states_.size());
//  dynamic_state.pDynamicStates = dynamic_states_.data();
//
//  // ======================== Vertex Input Create Info ========================
//  VkPipelineVertexInputStateCreateInfo vertex_input_state;
//  vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//  vertex_input_state.pNext = nullptr;
//  vertex_input_state.flags = 0;
//  vertex_input_state.vertexBindingDescriptionCount = 0;
//  vertex_input_state.pVertexBindingDescriptions = nullptr;
//  vertex_input_state.vertexAttributeDescriptionCount = 0;
//  vertex_input_state.pVertexAttributeDescriptions = nullptr;
//
//  // ======================== Input Assembly Create Info ========================
//  VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
//  input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//  input_assembly_state.pNext = nullptr;
//  input_assembly_state.flags = 0;
//  input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//  input_assembly_state.primitiveRestartEnable = VK_FALSE;
//
//  // ======================== Viewport ========================
//  VkViewport viewport;
//  viewport.x = viewport.y = 0.0f;
//  viewport.width = (float) swap_chain_details.swap_chain_extend_2d.width;
//  viewport.height = (float) swap_chain_details.swap_chain_extend_2d.height;
//  viewport.minDepth = 0.0f;
//  viewport.maxDepth = 1.0f;
//
//  // ======================== Scissor ========================
//  VkRect2D scissor;
//  scissor.offset = {0, 0};
//  scissor.extent = swap_chain_details.swap_chain_extend_2d;
//
//  // ======================== Viewport State Create Info ========================
//  VkPipelineViewportStateCreateInfo viewport_state;
//  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//  viewport_state.pNext = nullptr;
//  viewport_state.flags = 0;
//  viewport_state.viewportCount = 1;
//  viewport_state.pViewports = &viewport;
//  viewport_state.scissorCount = 1;
//  viewport_state.pScissors = &scissor;
//
//  // ======================== Rasterizer Create Info ========================
//  VkPipelineRasterizationStateCreateInfo rasterizer;
//  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//  rasterizer.pNext = nullptr;
//  rasterizer.flags = 0;
//  rasterizer.depthClampEnable = VK_FALSE;
//  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//  rasterizer.lineWidth = 1.0f;
//  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//  rasterizer.depthBiasEnable = VK_FALSE;
//  rasterizer.depthBiasConstantFactor = 0.0f;
//  rasterizer.depthBiasClamp = 0.0f;
//  rasterizer.depthBiasSlopeFactor = 0.0f;
//
//  // ======================== Multisampling Create Info ========================
//  VkPipelineMultisampleStateCreateInfo multisample_state_info;
//  multisample_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//  multisample_state_info.pNext = nullptr;
//  multisample_state_info.flags = 0;
//  multisample_state_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//  multisample_state_info.sampleShadingEnable = VK_FALSE;
//  multisample_state_info.minSampleShading = 1.0f;
//  multisample_state_info.pSampleMask = nullptr;
//  multisample_state_info.alphaToCoverageEnable = VK_FALSE;
//  multisample_state_info.alphaToOneEnable = VK_FALSE;
//
//  // ======================== Color Blending ========================
//  VkPipelineColorBlendAttachmentState color_blend_attachment;
//  color_blend_attachment.blendEnable = VK_FALSE;
//  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
//  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
//  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
//  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
//
//  // ======================== Color Blending ========================
//  VkPipelineColorBlendStateCreateInfo color_blending{};
//  color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//  color_blending.pNext = nullptr;
//  color_blending.flags = 0;
//  color_blending.logicOpEnable = VK_FALSE;
//  color_blending.logicOp = VK_LOGIC_OP_COPY;
//  color_blending.attachmentCount = 1;
//  color_blending.pAttachments = &color_blend_attachment;
//  color_blending.blendConstants[0] = 0.0f;
//  color_blending.blendConstants[1] = 0.0f;
//  color_blending.blendConstants[2] = 0.0f;
//  color_blending.blendConstants[3] = 0.0f;
//
//  // ======================== Pipeline Layout Create Info ========================
//  VkPipelineLayoutCreateInfo pipeline_layout_info;
//  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//  pipeline_layout_info.pNext = nullptr;
//  pipeline_layout_info.flags = 0;
//  pipeline_layout_info.setLayoutCount = 0;
//  pipeline_layout_info.pSetLayouts = nullptr;
//  pipeline_layout_info.pushConstantRangeCount = 0;
//  pipeline_layout_info.pPushConstantRanges = nullptr;
//
//  VkResult result = vkCreatePipelineLayout(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
//                                           &pipeline_layout_info,
//                                           nullptr,
//                                           &vk_pipeline_layout_);
//  if (result != VK_SUCCESS) {
//    ARIA_CORE_ERROR("Failed to create pipeline layout - {0}", string_VkResult(result))
//  }
//
//  VkGraphicsPipelineCreateInfo pipeline_info{};
//  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//  pipeline_info.stageCount = static_cast<std::uint32_t>(shader_stages_.size());
//  pipeline_info.pStages = shader_stages_.data();
//  pipeline_info.pVertexInputState = &vertex_input_state;
//  pipeline_info.pInputAssemblyState = &input_assembly_state;
//  pipeline_info.pViewportState = &viewport_state;
//  pipeline_info.pRasterizationState = &rasterizer;
//  pipeline_info.pMultisampleState = &multisample_state_info;
//  pipeline_info.pDepthStencilState = nullptr;
//  pipeline_info.pColorBlendState = &color_blending;
//  pipeline_info.pDynamicState = &dynamic_state;
//
//  pipeline_info.layout = vk_pipeline_layout_;
//  pipeline_info.renderPass = VulkanRendererApi::GetInstance().GetRenderPass();
//  pipeline_info.subpass = 0;
//
//  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
//  pipeline_info.basePipelineIndex = -1;
//
//  result = vkCreateGraphicsPipelines(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
//                                     VK_NULL_HANDLE,
//                                     1,
//                                     &pipeline_info,
//                                     nullptr,
//                                     &vk_graphics_pipeline_);
//  if (result != VK_SUCCESS) {
//    ARIA_CORE_ERROR("Failed to create graphics pipeline - {0}", string_VkResult(result))
//  }
//}

//void VulkanGraphicsPipeline::CreateFrameBuffers() {
//  VulkanSwapChain::VulkanSwapChainDetails details = VulkanSwapChain::GetInstance().GetSwapChainDetails();
//
//  vk_frame_buffers_.resize(details.swap_chain_images.size());
//
//  for (size_t i = 0; i < details.swap_chain_image_views.size(); i++) {
//    std::array<VkImageView, 1> attachments = {details.swap_chain_image_views[i]};
//
//    VkFramebufferCreateInfo frame_buffer_info;
//    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//    frame_buffer_info.flags = 0;
//    frame_buffer_info.pNext = nullptr;
//    frame_buffer_info.renderPass = VulkanRendererApi::GetInstance().GetRenderPass();
//    frame_buffer_info.attachmentCount = attachments.size();
//    frame_buffer_info.pAttachments = attachments.data();
//    frame_buffer_info.width = details.swap_chain_extend_2d.width;
//    frame_buffer_info.height = details.swap_chain_extend_2d.height;
//    frame_buffer_info.layers = 1;
//
//    VkResult result = vkCreateFramebuffer(VulkanDeviceManager::GetInstance().GetLogicalDevice(),
//                                          &frame_buffer_info,
//                                          nullptr,
//                                          &vk_frame_buffers_[i]);
//    if (result != VK_SUCCESS) {
//      ARIA_CORE_ERROR("Failed to create frame buffer - {0}", string_VkResult(result))
//    }
//  }
//}

VkPipeline VulkanGraphicsPipeline::GetGraphicsPipeline() const { return vk_graphics_pipeline_; }

std::vector<VkFramebuffer> VulkanGraphicsPipeline::GetFrameBuffers() const { return vk_frame_buffers_; }
}// namespace aria

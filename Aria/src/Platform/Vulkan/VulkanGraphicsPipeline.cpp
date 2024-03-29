#include "ariapch.h"
#include "VulkanGraphicsPipeline.h"
#include "Aria/Core/Log.h"
#include "VulkanDeviceManager.h"
#include "VulkanError.h"
#include "VulkanLib.h"
#include "VulkanRendererApi.h"
#include "vulkan/vk_enum_string_helper.h"

namespace aria {

std::vector<VkPipelineShaderStageCreateInfo> VulkanGraphicsPipeline::shader_stages_ = {};

VulkanGraphicsPipeline::VulkanGraphicsPipeline(Ref<VulkanDeviceManager> device_manager)
    : vk_graphics_pipeline_(nullptr), vk_pipeline_layout_(nullptr), p_vk_render_pass_(nullptr), vk_frame_buffers_({}),
      p_vulkan_device_manager_(device_manager) {
  //  vk_pipeline_cache_ = nullptr;
  Init();
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
  auto const &vklib = VulkanLib::GetInstance();
  for (auto buffer : vk_frame_buffers_) {
    vklib.ptr_vk_destroy_framebuffer(p_vulkan_device_manager_->GetLogicalDevice(), buffer, nullptr);
  }

  //  vklib.ptr_vk_destroy_pipeline_cache(VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_pipeline_cache_,
  //                                      nullptr);
  vklib.ptr_vk_destroy_pipeline(p_vulkan_device_manager_->GetLogicalDevice(), vk_graphics_pipeline_, nullptr);
  vklib.ptr_vk_destroy_pipeline_layout(p_vulkan_device_manager_->GetLogicalDevice(), vk_pipeline_layout_, nullptr);
}

Ref<VulkanGraphicsPipeline> VulkanGraphicsPipeline::Create(Ref<VulkanDeviceManager> device_manager) {
  return std::make_shared<VulkanGraphicsPipeline>(device_manager);
}

void VulkanGraphicsPipeline::Init() {
  p_vk_render_pass_ = VulkanRenderPass::Create(p_vulkan_device_manager_);
  //  InitPipelineCache();
  // Check if shader stages have their modules set
  if (IsAllModulesSet()) { CreateGraphicsPipeline(); }
  CreateFrameBuffers();
}

void VulkanGraphicsPipeline::CreateGraphicsPipeline() {
  vkb::Swapchain swapchain = p_vulkan_device_manager_->GetSwapChain();

  // ======================== Dynamic Create Info ========================

  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = static_cast<uint32_t>(kDynamicStates.size());
  dynamic_state.pDynamicStates = kDynamicStates.data();

  // ======================== Vertex Input Create Info ========================
  VkPipelineVertexInputStateCreateInfo vertex_input_state{};
  vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state.pNext = nullptr;
  vertex_input_state.flags = 0;
  vertex_input_state.vertexBindingDescriptionCount = 0;
  vertex_input_state.pVertexBindingDescriptions = nullptr;
  vertex_input_state.vertexAttributeDescriptionCount = 0;
  vertex_input_state.pVertexAttributeDescriptions = nullptr;

  // ======================== Input Assembly Create Info ========================
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state{};
  input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_state.pNext = nullptr;
  input_assembly_state.flags = 0;
  input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_state.primitiveRestartEnable = VK_FALSE;

  // ======================== Viewport ========================
  VkViewport viewport;
  viewport.x = viewport.y = 0.0f;
  viewport.width = (float) swapchain.extent.width;
  viewport.height = (float) swapchain.extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // ======================== Scissor ========================
  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = swapchain.extent;

  // ======================== Viewport State Create Info ========================
  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.pNext = nullptr;
  viewport_state.flags = 0;
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  // ======================== Rasterizer Create Info ========================
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.pNext = nullptr;
  rasterizer.flags = 0;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  // ======================== Multisampling Create Info ========================
  VkPipelineMultisampleStateCreateInfo multisample_state_info{};
  multisample_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample_state_info.pNext = nullptr;
  multisample_state_info.flags = 0;
  multisample_state_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisample_state_info.sampleShadingEnable = VK_FALSE;
  multisample_state_info.minSampleShading = 1.0f;
  multisample_state_info.pSampleMask = nullptr;
  multisample_state_info.alphaToCoverageEnable = VK_FALSE;
  multisample_state_info.alphaToOneEnable = VK_FALSE;

  // ======================== Color Blending ========================
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  // ======================== Color Blending ========================
  VkPipelineColorBlendStateCreateInfo color_blending{};
  color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.pNext = nullptr;
  color_blending.flags = 0;
  color_blending.logicOpEnable = VK_FALSE;
  color_blending.logicOp = VK_LOGIC_OP_COPY;
  color_blending.attachmentCount = 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f;
  color_blending.blendConstants[1] = 0.0f;
  color_blending.blendConstants[2] = 0.0f;
  color_blending.blendConstants[3] = 0.0f;

  // ======================== Pipeline Layout Create Info ========================
  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.pNext = nullptr;
  pipeline_layout_info.flags = 0;
  pipeline_layout_info.setLayoutCount = 0;
  pipeline_layout_info.pSetLayouts = nullptr;
  pipeline_layout_info.pushConstantRangeCount = 0;
  pipeline_layout_info.pPushConstantRanges = nullptr;

  VkResult result = VulkanLib::GetInstance().ptr_vk_create_pipeline_layout(
      p_vulkan_device_manager_->GetLogicalDevice(), &pipeline_layout_info, nullptr, &vk_pipeline_layout_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create pipeline layout")

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.flags = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
  pipeline_info.stageCount = static_cast<std::uint32_t>(shader_stages_.size());
  pipeline_info.pStages = shader_stages_.data();
  pipeline_info.pVertexInputState = &vertex_input_state;
  pipeline_info.pInputAssemblyState = &input_assembly_state;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisample_state_info;
  pipeline_info.pDepthStencilState = nullptr;
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = &dynamic_state;

  pipeline_info.layout = vk_pipeline_layout_;
  pipeline_info.renderPass = p_vk_render_pass_->GetVkRenderPass();
  pipeline_info.subpass = 0;

  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_info.basePipelineIndex = -1;

  result = VulkanLib::GetInstance().ptr_vk_create_graphics_pipelines(
      p_vulkan_device_manager_->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vk_graphics_pipeline_);
  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create graphics pipeline")
}

void VulkanGraphicsPipeline::CreateFrameBuffers() {
  auto swapchain = p_vulkan_device_manager_->GetSwapChain();

  auto image_views_ret = swapchain.get_image_views();
  ARIA_VKB_CHECK_RESULT_AND_ASSERT(image_views_ret, "Failed to get image views");
  vk_frame_buffers_.resize(swapchain.image_count);

  for (size_t i = 0; i < swapchain.image_count; i++) {
    std::array<VkImageView, 1> attachments = {image_views_ret.value()[i]};

    VkFramebufferCreateInfo frame_buffer_info{};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.flags = 0;
    frame_buffer_info.pNext = nullptr;
    frame_buffer_info.renderPass = p_vk_render_pass_->GetVkRenderPass();
    frame_buffer_info.attachmentCount = attachments.size();
    frame_buffer_info.pAttachments = attachments.data();
    frame_buffer_info.width = swapchain.extent.width;
    frame_buffer_info.height = swapchain.extent.height;
    frame_buffer_info.layers = 1;

    VkResult result = VulkanLib::GetInstance().ptr_vk_create_framebuffer(
        p_vulkan_device_manager_->GetLogicalDevice(), &frame_buffer_info, nullptr, &vk_frame_buffers_[i]);
    ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create frame buffer")
  }
}

void VulkanGraphicsPipeline::AddToShaderStages(VkShaderModule &shader_module, ShaderType type) {
  VkPipelineShaderStageCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  create_info.flags = 0;
  create_info.pNext = nullptr;

  switch (type) {
    case ShaderType::VERTEX:
      create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case ShaderType::FRAGMENT:
      create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader type; cannot create VkShaderModule")
      break;
  }
  create_info.module = shader_module;
  create_info.pName = "main";
  create_info.pSpecializationInfo = nullptr;

  shader_stages_.push_back(create_info);

  if (IsAllModulesSet()) {
    if (vk_graphics_pipeline_ == nullptr) {
      CreateGraphicsPipeline();
    } else {
      DestroyPipeline();
      CreateGraphicsPipeline();
      // UpdatePipeline();
    }
  }
}

//void VulkanGraphicsPipeline::InitPipelineCache() {
//  VkPipelineCacheCreateInfo pipeline_cache_info = {};
//  pipeline_cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
//
//  VkResult result = VulkanLib::GetInstance().ptr_vk_create_pipeline_cache(
//      VulkanDeviceManager::GetInstance().GetLogicalDevice(), &pipeline_cache_info, nullptr, &vk_pipeline_cache_);
//  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to create pipeline cache")
//}

void VulkanGraphicsPipeline::DestroyPipeline() {
  Ref<VulkanDeviceManager> device_manager = VulkanRendererApi::GetInstance().GetVulkanDeviceManager();

  VulkanLib::GetInstance().ptr_vk_destroy_pipeline(device_manager->GetLogicalDevice(), vk_graphics_pipeline_, nullptr);
}

//void VulkanGraphicsPipeline::UpdatePipeline() {
//  VkGraphicsPipelineCreateInfo pipeline_info{};
//  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//  pipeline_info.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
//  pipeline_info.pNext = nullptr;
//  pipeline_info.basePipelineHandle = vk_graphics_pipeline_;
//  pipeline_info.basePipelineIndex = -1;
//
//  pipeline_info.stageCount = static_cast<std::uint32_t>(shader_stages_.size());
//  pipeline_info.pStages = shader_stages_.data();
//
//  VkResult result = VulkanLib::GetInstance().ptr_vk_create_graphics_pipelines(
//      VulkanDeviceManager::GetInstance().GetLogicalDevice(), vk_pipeline_cache_, 1, &pipeline_info, nullptr,
//      &vk_graphics_pipeline_);
//  ARIA_VK_CHECK_RESULT_AND_ERROR(result, "Failed to update graphics pipeline")
//}

bool VulkanGraphicsPipeline::IsAllModulesSet() {
  if (shader_stages_.empty()) { return false; }

  return std::all_of(shader_stages_.begin(), shader_stages_.end(),
                     [](const VkPipelineShaderStageCreateInfo &stage) { return stage.module != VK_NULL_HANDLE; });
}
}// namespace aria

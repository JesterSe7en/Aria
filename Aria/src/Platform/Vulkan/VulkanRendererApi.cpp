#include "ariapch.h"

#include "Platform/Vulkan/VulkanRendererApi.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Base.h"
#include "Aria/Core/Log.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "GLFW/glfw3.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_core.h"

#include <fileapi.h>

#include <array>
#include <limits>
#include <set>
#include <vector>

namespace aria {

VkInstance VulkanRendererApi::p_vk_instance_ = nullptr;
std::vector<VkShaderModule> VulkanRendererApi::shader_modules_ = {};
std::vector<VkPipelineShaderStageCreateInfo> VulkanRendererApi::shader_stages_ = {};

VulkanRendererApi::~VulkanRendererApi() { Cleanup(); }

void VulkanRendererApi::Init() {
  CreateInstance();
  CreatePresentationSurface();

  vulkan_debug_messenger_.Init();
  VulkanDeviceManager::GetInstance().Init();

  CreateSwapChain();
  CreateImageViews();
  CreateRenderPass();
  CreateCommandPool();
  // create_graphics_pipeline();
}
void VulkanRendererApi::Clear() {ARIA_CORE_ASSERT(false, "Not Implemented")}

void VulkanRendererApi::SetClearColor(const glm::vec4 color) {ARIA_CORE_ASSERT(false, "Not Implemented")}

void VulkanRendererApi::DrawIndexed(const Ref<VertexArray> &vertex_array) {
  ARIA_CORE_ASSERT(false, "Not Implemented")
}

void VulkanRendererApi::AddToPipeline(VkShaderModule &shader_module, ShaderType type) {
  VkPipelineShaderStageCreateInfo create_info;
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
    default: ARIA_CORE_ASSERT(false, "Unknown shader type; cannot create VkShaderModule")
      break;
  }
  create_info.module = shader_module;
  create_info.pName = "main";
  create_info.pSpecializationInfo = nullptr;
  shader_stages_.push_back(create_info);
}

void VulkanRendererApi::CreatePipeline() {
  CreateGraphicsPipeline();
  CreateFrameBuffers();
}

void VulkanRendererApi::CreateInstance() {
  if (VulkanRendererApi::IsValidationLayersEnabled() && !HasValidationSupport()) {
    ARIA_CORE_WARN("Vulkan validation layers requested, but none available")
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Aria Vulkan Application";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Aria";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_VERSION_1_3;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  std::vector<const char *> extensions = GetGlfwRequiredExtensions();

  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = 0;

  // setup another create info struct to capture events during creation and destruction of VKInstance
  // Vulkan 1.3 spec pg. 3921
  if (VulkanRendererApi::IsValidationLayersEnabled()) {
    create_info.enabledLayerCount = validation_layers_.size();
    create_info.ppEnabledLayerNames = validation_layers_.data();
    create_info.pNext = &VulkanDebugMessenger::GetDebugMessengerCreateInfo();
  } else {
    create_info.enabledLayerCount = 0;
    create_info.pNext = nullptr;
  }

  // query the supported instance extensions
  // std::uint32_t numInstanceExtensions = 0;
  // std::vector<VkExtensionProperties> instanceExtensionProperties;
  // Query the instance extensions.
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, nullptr);

  // If there are any extensions, query their properties.

  // if (numInstanceExtensions != 0) {
  // instanceExtensionProperties.resize(numInstanceExtensions);
  // vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, instanceExtensionProperties.data());
  // }
  // if instance extensions are enabled, look for function pointers via
  // vkGetInstanceProcAddr() func

  VkResult result = vkCreateInstance(&create_info, nullptr, &p_vk_instance_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreatePresentationSurface() {
  ARIA_CORE_ASSERT(p_vk_instance_, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  VkResult result = glfwCreateWindowSurface(p_vk_instance_, glfw_window, nullptr, &surface_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create vulkan surface - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreateSwapChain() {

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
  create_info.surface = surface_;
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
      result = vkCreateSwapchainKHR(VulkanDeviceManager::GetLogicalDevice(), &create_info, nullptr, &swapchain_khr_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create swap chain - {0}", string_VkResult(result))
  }

  vkGetSwapchainImagesKHR(VulkanDeviceManager::GetLogicalDevice(), swapchain_khr_, &image_count, nullptr);
  swap_chain_images_.resize(image_count);
  vkGetSwapchainImagesKHR(VulkanDeviceManager::GetLogicalDevice(),
                          swapchain_khr_,
                          &image_count,
                          swap_chain_images_.data());

  swap_chain_format_ = surface_format.format;
  swap_chain_extent_2d_ = extent;
}

void VulkanRendererApi::CreateImageViews() {
  swap_chain_image_views_.resize(swap_chain_images_.size());

  for (size_t idx = 0; idx < swap_chain_images_.size(); idx++) {
    VkImageViewCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.image = swap_chain_images_[idx];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = swap_chain_format_;

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
                                        &swap_chain_image_views_[idx]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create image views - {0}", string_VkResult(result))
    }
  }
}

void VulkanRendererApi::CreateRenderPass() {
  VkAttachmentDescription color_attachment;
  color_attachment.format = swap_chain_format_;
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

  VkResult
      result = vkCreateRenderPass(VulkanDeviceManager::GetLogicalDevice(), &render_pass_info, nullptr, &render_pass_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create render pass - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreateGraphicsPipeline() {
  //

  // ======================== Shader Create Info ========================
  // VkPipelineShaderStageCreateInfo vertex_shader_stage_info;
  // vertex_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  // vertex_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  // // TODO: vertex_shader_stage_info.module = Need to grab vertex shader from ShaderLibrary in VulkanLayer
  // vertex_shader_stage_info.pName = "main";

  // VkPipelineShaderStageCreateInfo frag_shader_stage_info;
  // frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  // frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  // // TODO: frag_shader_stage_info.module = Need to grab frag shader from ShaderLibrary in VulkanLayer
  // frag_shader_stage_info.pName = "main";

  // std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages = {vertex_shader_stage_info, frag_shader_stage_info};

  // ======================== Dynamic Create Info ========================

  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states_.size());
  dynamic_state.pDynamicStates = dynamic_states_.data();

  // ======================== Vertex Input Create Info ========================
  VkPipelineVertexInputStateCreateInfo vertex_input_state;
  vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state.pNext = nullptr;
  vertex_input_state.flags = 0;
  vertex_input_state.vertexBindingDescriptionCount = 0;
  vertex_input_state.pVertexBindingDescriptions = nullptr;
  vertex_input_state.vertexAttributeDescriptionCount = 0;
  vertex_input_state.pVertexAttributeDescriptions = nullptr;

  // ======================== Input Assembly Create Info ========================
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
  input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_state.pNext = nullptr;
  input_assembly_state.flags = 0;
  input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_state.primitiveRestartEnable = VK_FALSE;

  // ======================== Viewport ========================
  VkViewport viewport;
  viewport.x = viewport.y = 0.0f;
  viewport.width = (float) swap_chain_extent_2d_.width;
  viewport.height = (float) swap_chain_extent_2d_.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // ======================== Scissor ========================
  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = swap_chain_extent_2d_;

  // ======================== Viewport State Create Info ========================
  VkPipelineViewportStateCreateInfo viewport_state;
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.pNext = nullptr;
  viewport_state.flags = 0;
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  // ======================== Rasterizer Create Info ========================
  VkPipelineRasterizationStateCreateInfo rasterizer;
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.pNext = nullptr;
  rasterizer.flags = 0;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  // ======================== Multisampling Create Info ========================
  VkPipelineMultisampleStateCreateInfo multisample_state_info;
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
  VkPipelineColorBlendAttachmentState color_blend_attachment;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

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
  VkPipelineLayoutCreateInfo pipeline_layout_info;
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.pNext = nullptr;
  pipeline_layout_info.flags = 0;
  pipeline_layout_info.setLayoutCount = 0;
  pipeline_layout_info.pSetLayouts = nullptr;
  pipeline_layout_info.pushConstantRangeCount = 0;
  pipeline_layout_info.pPushConstantRanges = nullptr;

  VkResult result = vkCreatePipelineLayout(VulkanDeviceManager::GetLogicalDevice(),
                                           &pipeline_layout_info,
                                           nullptr,
                                           &pipeline_layout_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create pipeline layout - {0}", string_VkResult(result))
  }

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
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

  pipeline_info.layout = pipeline_layout_;
  pipeline_info.renderPass = render_pass_;
  pipeline_info.subpass = 0;

  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_info.basePipelineIndex = -1;

  result = vkCreateGraphicsPipelines(VulkanDeviceManager::GetLogicalDevice(),
                                     VK_NULL_HANDLE,
                                     1,
                                     &pipeline_info,
                                     nullptr,
                                     &graphics_pipeline_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create graphics pipeline - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::CreateFrameBuffers() {
  swap_chain_frame_buffers_.resize(swap_chain_images_.size());

  for (size_t i = 0; i < swap_chain_image_views_.size(); i++) {
    std::array<VkImageView, 1> attachments = {swap_chain_image_views_[i]};

    VkFramebufferCreateInfo frame_buffer_info;
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.flags = 0;
    frame_buffer_info.pNext = nullptr;
    frame_buffer_info.renderPass = render_pass_;
    frame_buffer_info.attachmentCount = attachments.size();
    frame_buffer_info.pAttachments = attachments.data();
    frame_buffer_info.width = swap_chain_extent_2d_.width;
    frame_buffer_info.height = swap_chain_extent_2d_.height;
    frame_buffer_info.layers = 1;

    VkResult result = vkCreateFramebuffer(VulkanDeviceManager::GetLogicalDevice(),
                                          &frame_buffer_info,
                                          nullptr,
                                          &swap_chain_frame_buffers_[i]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create frame buffer - {0}", string_VkResult(result))
    }
  }
}

void VulkanRendererApi::CreateCommandPool() {

  VulkanDeviceManager::QueueFamilyIndices
      queue_family_indices = VulkanDeviceManager::GetInstance().GetQueueFamilyIndicies();

  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = 0;
  cmd_pool_info.pNext = nullptr;
  cmd_pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

  VkResult
      result = vkCreateCommandPool(VulkanDeviceManager::GetLogicalDevice(), &cmd_pool_info, nullptr, &command_pool_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command pool - {0}", string_VkResult(result))
  }
}

VkCommandBuffer VulkanRendererApi::CreateVkCommandBuffer() {
  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = command_pool_;
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = 1;

  VkResult
      result = vkAllocateCommandBuffers(VulkanDeviceManager::GetLogicalDevice(), &buffer_alloc_info, &command_buffer_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
    return nullptr;
  }
  return command_buffer_;
}

bool VulkanRendererApi::HasValidationSupport() {
  // how many instance layers can the vulkan system support?
  std::uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char *layer_name : validation_layers_) {
    bool layer_found = false;

    for (const auto &kLayerProperties : available_layers) {
      if (strcmp(layer_name, kLayerProperties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      return false;
    }
  }
  return true;
}

std::vector<const char *> VulkanRendererApi::GetGlfwRequiredExtensions() {
  std::uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  if (VulkanRendererApi::HasValidationSupport()) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

// By default, use VK_FORMAT_B8G8R8A8_SRGB and sRGB color space
VkSurfaceFormatKHR VulkanRendererApi::GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) {
  for (const auto &kFormat : formats) {
    if (kFormat.format == VK_FORMAT_B8G8R8A8_SRGB && kFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return kFormat;
    }
  }

  return formats[0];
}

VkPresentModeKHR VulkanRendererApi::GetPresentMode(const std::vector<VkPresentModeKHR> &modes) {
  for (const auto &kMode : modes) {
    if (kMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return kMode;
    }
  }
  // Only guaranteed this mode
  // vulkan spec v1.3 pg 2717
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRendererApi::GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
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

void VulkanRendererApi::Cleanup() {
  vkDestroyCommandPool(VulkanDeviceManager::GetLogicalDevice(), command_pool_, nullptr);

  for (auto buffer : swap_chain_frame_buffers_) {
    vkDestroyFramebuffer(VulkanDeviceManager::GetLogicalDevice(), buffer, nullptr);
  }
  vkDestroyPipeline(VulkanDeviceManager::GetLogicalDevice(), graphics_pipeline_, nullptr);
  vkDestroyPipelineLayout(VulkanDeviceManager::GetLogicalDevice(), pipeline_layout_, nullptr);
  vkDestroyRenderPass(VulkanDeviceManager::GetLogicalDevice(), render_pass_, nullptr);
  for (auto view : swap_chain_image_views_) {
    vkDestroyImageView(VulkanDeviceManager::GetLogicalDevice(), view, nullptr);
  }
  vkDestroySwapchainKHR(VulkanDeviceManager::GetLogicalDevice(), swapchain_khr_, nullptr);
  vkDestroySurfaceKHR(p_vk_instance_, surface_, nullptr);
  vkDestroyInstance(p_vk_instance_, nullptr);
}

}  // namespace ARIA

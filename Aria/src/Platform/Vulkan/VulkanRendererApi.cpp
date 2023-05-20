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

VkInstance VulkanRendererApi::p_vk_instance_t_ = nullptr;
VkDevice VulkanRendererApi::p_device_t_ = VK_NULL_HANDLE;
std::vector<VkShaderModule> VulkanRendererApi::shader_modules_ = {};
std::vector<VkPipelineShaderStageCreateInfo> VulkanRendererApi::shader_stages_ = {};

VulkanRendererApi::~VulkanRendererApi() { Cleanup(); }

void VulkanRendererApi::Init() {
  CreateInstance();
  SetupVulkanDebugMessenger();
  CreatePresentationSurface();
  PickPhysicalDevice();
  CreateLogicalDevice();
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
    case ShaderType::VERTEX:create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case ShaderType::FRAGMENT:create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
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
  if (kEnableValidationLayers && !HasValidationSupport()) {
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
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (kEnableValidationLayers) {
    create_info.enabledLayerCount = validation_layers_.size();
    create_info.ppEnabledLayerNames = validation_layers_.data();
    PopulateDebugCreateInfo(debug_create_info);
    create_info.pNext = &debug_create_info;
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

  VkResult result = vkCreateInstance(&create_info, nullptr, &p_vk_instance_t_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::SetupVulkanDebugMessenger() {
  if (!kEnableValidationLayers) {
    return;
  }
  ARIA_CORE_ASSERT(p_vk_instance_t_ != nullptr, "Did you create VkInstance before setting up debug messenger?")

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  PopulateDebugCreateInfo(create_info);

  if (CreateDebugUtilMessengerExt(p_vk_instance_t_, &create_info, nullptr, &debug_messenger_) != VK_SUCCESS) {
    ARIA_CORE_WARN("Cannot setup debug messenger; debug messenger extension not available")
  }
}

void VulkanRendererApi::CreatePresentationSurface() {
  ARIA_CORE_ASSERT(p_vk_instance_t_, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  VkResult result = glfwCreateWindowSurface(p_vk_instance_t_, glfw_window, nullptr, &surface_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create vulkan surface - {0}", string_VkResult(result))
  }
}

void VulkanRendererApi::PickPhysicalDevice() {
  std::uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(p_vk_instance_t_, &device_count, nullptr);

  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(p_vk_instance_t_, &device_count, devices.data());

  for (const auto &kDevice : devices) {
    if (IsSuitableVulkanDevice(kDevice)) {
      physical_device_ = kDevice;
      break;
    }
  }

  ARIA_CORE_ASSERT(physical_device_ != VK_NULL_HANDLE,
                   "Found GPUs with Vulkan support, but no suitable devices for Aria Engine")

  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(physical_device_, &properties);

  ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
  ARIA_CORE_INFO("Name: {0}", properties.deviceName)
  ARIA_CORE_INFO("Driver version: {0}", properties.driverVersion)

  // features can include optional capabilities such as geometry shaders, tessellation shaders, multi-viewport
  // rendering, texture compression formats, and more.

  // extensions
  std::uint32_t extensions_count = 0;
  vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &extensions_count, nullptr);

  std::vector<VkExtensionProperties> extensions(extensions_count);
  vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &extensions_count, extensions.data());

  ARIA_CORE_INFO("Available extensions for physical device:")
  for (const auto &kExtension : extensions) {
    ARIA_CORE_INFO(kExtension.extensionName)
  }

  // to call device extensions funcs
  // vkGetDeviceProcAddr()

  // physical device layers
  std::uint32_t layer_count;
  vkEnumerateDeviceLayerProperties(physical_device_, &layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateDeviceLayerProperties(physical_device_, &layer_count, available_layers.data());
  // TODO: get driver version
  //  check layers' description for human-readable driver version
  //  NVIDIA: VK_LAYER_NV_optimus
  //  AMD: VK_LAYER_AMD_switchable_graphics
  //  INTEL: VK_LAYER_KHRONOS_validation
}

void VulkanRendererApi::CreateLogicalDevice() {
  QueryFamilyIndices indices = QueryQueueFamilies(physical_device_);

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<std::uint32_t> unique_queue_families = {indices.graphics_family.value(), indices.present_family.value()};

  float queue_priority = 1.0f;
  for (std::uint32_t queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();

  VkPhysicalDeviceFeatures device_features{};
  create_info.pEnabledFeatures = &device_features;

  if (CheckDeviceExtensionsSupport(physical_device_)) {
    create_info.ppEnabledExtensionNames = device_extensions_.data();
    create_info.enabledExtensionCount = static_cast<std::uint32_t>(device_extensions_.size());
  } else {
    create_info.enabledExtensionCount = 0;
  }

  if (kEnableValidationLayers) {
    create_info.enabledLayerCount = static_cast<std::uint32_t>(validation_layers_.size());
    create_info.ppEnabledLayerNames = validation_layers_.data();
  } else {
    create_info.enabledLayerCount = 0;
  }

  VkResult result = vkCreateDevice(physical_device_, &create_info, nullptr, &p_device_t_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create logical device - {0}", string_VkResult(result))
  }

  vkGetDeviceQueue(p_device_t_, indices.graphics_family.value(), 0, &graphics_queue_);
  vkGetDeviceQueue(p_device_t_, indices.present_family.value(), 0, &present_queue_);
}

void VulkanRendererApi::CreateSwapChain() {
  SwapChainDetails details = QuerySwapChainSupport(physical_device_);

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

  VulkanRendererApi::QueryFamilyIndices indices = QueryQueueFamilies(physical_device_);
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

  VkResult result = vkCreateSwapchainKHR(p_device_t_, &create_info, nullptr, &swapchain_khr_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create swap chain - {0}", string_VkResult(result))
  }

  vkGetSwapchainImagesKHR(p_device_t_, swapchain_khr_, &image_count, nullptr);
  swap_chain_images_.resize(image_count);
  vkGetSwapchainImagesKHR(p_device_t_, swapchain_khr_, &image_count, swap_chain_images_.data());

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

    VkResult result = vkCreateImageView(p_device_t_, &create_info, nullptr, &swap_chain_image_views_[idx]);
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

  VkResult result = vkCreateRenderPass(p_device_t_, &render_pass_info, nullptr, &render_pass_);
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

  VkResult result = vkCreatePipelineLayout(p_device_t_, &pipeline_layout_info, nullptr, &pipeline_layout_);
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

  result = vkCreateGraphicsPipelines(p_device_t_, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline_);
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

    VkResult result = vkCreateFramebuffer(p_device_t_, &frame_buffer_info, nullptr, &swap_chain_frame_buffers_[i]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create frame buffer - {0}", string_VkResult(result))
    }
  }
}

void VulkanRendererApi::CreateCommandPool() {
  QueryFamilyIndices query_family_indices = QueryQueueFamilies(physical_device_);

  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = 0;
  cmd_pool_info.pNext = nullptr;
  cmd_pool_info.queueFamilyIndex = query_family_indices.graphics_family.value();

  VkResult result = vkCreateCommandPool(p_device_t_, &cmd_pool_info, nullptr, &command_pool_);
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

  VkResult result = vkAllocateCommandBuffers(p_device_t_, &buffer_alloc_info, &command_buffer_);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
    return nullptr;
  }
  return command_buffer_;
}

bool VulkanRendererApi::HasValidationSupport() const {
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

void VulkanRendererApi::PopulateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info) const {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = VulkanLogCallback;
}

VkResult VulkanRendererApi::CreateDebugUtilMessengerExt(VkInstance instance,
                                                        const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
                                                        const VkAllocationCallbacks *p_allocator,
                                                        VkDebugUtilsMessengerEXT *p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRendererApi::VulkanLogCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data, void *p_user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:ARIA_CORE_INFO("Vulkan {0} Info: {1}",
                                                                     GetMessageType(message_type),
                                                                     p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:ARIA_CORE_TRACE("Vulkan {0} Trace: {1}",
                                                                         GetMessageType(message_type),
                                                                         p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:ARIA_CORE_ERROR("Vulkan {0} Error: {1}",
                                                                       GetMessageType(message_type),
                                                                       p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:ARIA_CORE_WARN("Vulkan {0} Error: {1}",
                                                                        GetMessageType(message_type),
                                                                        p_callback_data->pMessage)
      break;
    default: ARIA_CORE_ASSERT(false, "Unknown error type")
      break;
  }
  // per vulkan 1.3 spec, pg.3566
  // The callback returns a VkBool32, which is interpreted in a layer-specified manner. The application
  // should always return VK_FALSE. The VK_TRUE value is reserved for use in layer development.
  return VK_FALSE;
}

std::string VulkanRendererApi::GetMessageType(VkDebugUtilsMessageTypeFlagsEXT message_type) {
  switch (message_type) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:return "General";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:return "Performance";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:return "Validation";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:return "Device Address";
    default:return "Unknown Message Type";
  }
}

bool VulkanRendererApi::IsSuitableVulkanDevice(VkPhysicalDevice device) {
  QueryFamilyIndices queue_families = QueryQueueFamilies(device);

  bool swap_chain_supported = false;
  if (CheckDeviceExtensionsSupport(device)) {
    SwapChainDetails details = QuerySwapChainSupport(device);
    swap_chain_supported = !details.formats.empty() && !details.present_modes.empty();
  }

  return queue_families.IsComplete() && swap_chain_supported;

  // VkPhysicalDeviceProperties properties;
  // vkGetPhysicalDeviceProperties(device, &properties);

  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // as mentioned, can use scoring system to defer using Discrete GPU first
  // then integrated, or allow user to select
  // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

std::string VulkanRendererApi::GetVendorName(std::uint32_t vendor_id) const {
  switch (vendor_id) {
    case 0x10DE:return "NVIDIA";
    case 0x1002:return "AMD";
    case 0x8086:return "INTEL";
    default:return "Unknown";
  }
}

VulkanRendererApi::QueryFamilyIndices VulkanRendererApi::QueryQueueFamilies(VkPhysicalDevice device) {
  QueryFamilyIndices indices;

  std::uint32_t query_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> query_families(query_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, query_families.data());

  int i = 0;
  // All possible queue families
  // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html

  for (const auto &kQueueFamily : query_families) {
    VkBool32 surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &surface_supported);

    if (surface_supported) {
      indices.present_family = i;
    }

    if (kQueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family = i;
    }

    if (indices.IsComplete()) {
      break;
    }
    i++;
  }
  return indices;
}

VulkanRendererApi::SwapChainDetails VulkanRendererApi::QuerySwapChainSupport(VkPhysicalDevice device) {
  SwapChainDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

  std::uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, nullptr);
  if (format_count) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, details.formats.data());
  }

  std::uint32_t present_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_count, nullptr);
  if (present_count) {
    details.present_modes.resize(present_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_count, details.present_modes.data());
  }

  return details;
}

std::vector<const char *> VulkanRendererApi::GetGlfwRequiredExtensions() {
  std::uint32_t glfw_extension_count = 0;
  const char **glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  if (kEnableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool VulkanRendererApi::CheckDeviceExtensionsSupport(VkPhysicalDevice device) {
  std::uint32_t count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());

  std::set<std::string> required_extensions(device_extensions_.begin(), device_extensions_.end());

  for (const auto &kExtension : extensions) {
    required_extensions.erase(kExtension.extensionName);
  }
  return required_extensions.empty();
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
  vkDestroyCommandPool(p_device_t_, command_pool_, nullptr);

  for (auto buffer : swap_chain_frame_buffers_) {
    vkDestroyFramebuffer(p_device_t_, buffer, nullptr);
  }
  vkDestroyPipeline(p_device_t_, mGraphicsPipeline, nullptr);
  vkDestroyPipelineLayout(p_device_t_, pipeline_layout_, nullptr);
  vkDestroyRenderPass(p_device_t_, render_pass_, nullptr);
  for (auto view : swap_chain_image_views_) {
    vkDestroyImageView(p_device_t_, view, nullptr);
  }
  vkDestroySwapchainKHR(p_device_t_, swapchain_khr_, nullptr);
  vkDestroyDevice(p_device_t_, nullptr);

  if (kEnableValidationLayers) {
    auto func =
        (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(p_vk_instance_t_,
                                                                    "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(p_vk_instance_t_, debug_messenger_, nullptr);
    }
  }

  vkDestroySurfaceKHR(p_vk_instance_t_, surface_, nullptr);
  vkDestroyInstance(p_vk_instance_t_, nullptr);
}

}  // namespace ARIA

#include "ariapch.h"

#include "Platform/Vulkan/VulkanRendererAPI.h"

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

namespace ARIA {

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

VkInstance VulkanRendererAPI::sInstance = nullptr;
VkDevice VulkanRendererAPI::sDevice = VK_NULL_HANDLE;
std::vector<VkShaderModule> VulkanRendererAPI::sShaderModules = {};
std::vector<VkPipelineShaderStageCreateInfo> VulkanRendererAPI::sShaderStages = {};

VulkanRendererAPI::~VulkanRendererAPI() { cleanup(); }

void VulkanRendererAPI::init() {
  create_instance();
  setup_vulkan_debug_messenger();
  create_presentation_surface();
  pick_physical_device();
  create_logical_device();
  create_swap_chain();
  create_image_views();
  create_render_pass();
  create_command_pool();
  // create_graphics_pipeline();
}
void VulkanRendererAPI::clear() { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererAPI::set_clear_color(const glm::vec4 color) { ARIA_CORE_ASSERT(false, "Not Implemented") }

void VulkanRendererAPI::draw_indexed(const Ref<VertexArray>& vertex_array) {
  ARIA_CORE_ASSERT(false, "Not Implemented")
}

void VulkanRendererAPI::add_to_pipeline(VkShaderModule& shader_module, ShaderType type) {
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
    default:
      ARIA_CORE_ASSERT(false, "Unknown shader type; cannot create VkShaderModule")
      break;
  }
  create_info.module = shader_module;
  create_info.pName = "main";
  create_info.pSpecializationInfo = nullptr;
  sShaderStages.push_back(create_info);
}

void VulkanRendererAPI::create_pipeline() {
  create_graphics_pipeline();
  create_frame_buffers();
}

void VulkanRendererAPI::create_instance() {
  if (enable_validation_layers && !has_validation_support()) {
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

  std::vector<const char*> extensions = get_glfw_required_extensions();

  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = 0;

  // setup another create info struct to capture events during creation and destruction of VKInstance
  // Vulkan 1.3 spec pg. 3921
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (enable_validation_layers) {
    create_info.enabledLayerCount = mValidationLayers.size();
    create_info.ppEnabledLayerNames = mValidationLayers.data();
    populate_debug_create_info(debug_create_info);
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

  VkResult result = vkCreateInstance(&create_info, nullptr, &sInstance);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create vulkan instance - {0}", string_VkResult(result))
  }
}

void VulkanRendererAPI::setup_vulkan_debug_messenger() {
  if (!enable_validation_layers) {
    return;
  }
  ARIA_CORE_ASSERT(sInstance != nullptr, "Did you create VkInstance before setting up debug messenger?")

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  populate_debug_create_info(create_info);

  if (create_debug_util_messenger_ext(sInstance, &create_info, nullptr, &mDebugMessenger) != VK_SUCCESS) {
    ARIA_CORE_WARN("Cannot setup debug messenger; debug messenger extension not available")
  }
}

void VulkanRendererAPI::create_presentation_surface() {
  ARIA_CORE_ASSERT(sInstance, "Did you create VkInstance first?")
  auto glfw_window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
  ARIA_CORE_ASSERT(glfw_window, "Did you create window first before creating surface?")
  VkResult result = glfwCreateWindowSurface(sInstance, glfw_window, nullptr, &mSurface);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create vulkan surface - {0}", string_VkResult(result))
  }
}

void VulkanRendererAPI::pick_physical_device() {
  std::uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(sInstance, &device_count, nullptr);

  ARIA_CORE_ASSERT(device_count != 0, "You tried to setup with Vulkan API, but no GPU's found with Vulkan support")

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(sInstance, &device_count, devices.data());

  for (const auto& device : devices) {
    if (is_suitable_vulkan_device(device)) {
      mPhysicalDevice = device;
      break;
    }
  }

  ARIA_CORE_ASSERT(mPhysicalDevice != VK_NULL_HANDLE,
                   "Found GPUs with Vulkan support, but no suitable devices for Aria Engine")

  VkPhysicalDeviceProperties properties;
  vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);

  ARIA_CORE_INFO("--- Vulkan GUI Device --- ")
  ARIA_CORE_INFO("Name: {0}", properties.deviceName)
  ARIA_CORE_INFO("Driver version: {0}", properties.driverVersion)

  // features can include optional capabilities such as geometry shaders, tessellation shaders, multi-viewport
  // rendering, texture compression formats, and more.

  // extensions
  std::uint32_t extensions_count = 0;
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, nullptr);

  std::vector<VkExtensionProperties> extensions(extensions_count);
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensions_count, extensions.data());

  ARIA_CORE_INFO("Available extensions for physical device:")
  for (const auto& extension : extensions) {
    ARIA_CORE_INFO(extension.extensionName)
  }

  // to call device extensions funcs
  // vkGetDeviceProcAddr()

  // physical device layers
  std::uint32_t layerCount;
  vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, availableLayers.data());
  // TODO: get driver version
  //  check layers' description for human-readable driver version
  //  NVIDIA: VK_LAYER_NV_optimus
  //  AMD: VK_LAYER_AMD_switchable_graphics
  //  INTEL: VK_LAYER_KHRONOS_validation
}

void VulkanRendererAPI::create_logical_device() {
  QueryFamilyIndices indices = query_queue_families(mPhysicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (std::uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queueFamily;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queuePriority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();

  VkPhysicalDeviceFeatures deviceFeatures{};
  create_info.pEnabledFeatures = &deviceFeatures;

  if (check_device_extensions_support(mPhysicalDevice)) {
    create_info.ppEnabledExtensionNames = mDeviceExtensions.data();
    create_info.enabledExtensionCount = static_cast<std::uint32_t>(mDeviceExtensions.size());
  } else {
    create_info.enabledExtensionCount = 0;
  }

  if (enable_validation_layers) {
    create_info.enabledLayerCount = static_cast<std::uint32_t>(mValidationLayers.size());
    create_info.ppEnabledLayerNames = mValidationLayers.data();
  } else {
    create_info.enabledLayerCount = 0;
  }

  VkResult result = vkCreateDevice(mPhysicalDevice, &create_info, nullptr, &sDevice);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Cannot create logical device - {0}", string_VkResult(result))
  }

  vkGetDeviceQueue(sDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
  vkGetDeviceQueue(sDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

void VulkanRendererAPI::create_swap_chain() {
  SwapChainDetails details = query_swap_chain_support(mPhysicalDevice);

  VkSurfaceFormatKHR surface_format = get_swap_surface_format(details.formats);
  VkPresentModeKHR present_mode = get_present_mode(details.presentModes);
  VkExtent2D extent = get_swap_extent(details.capabilities);

  std::uint32_t image_count = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
    image_count = details.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info;
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.pNext = nullptr;
  create_info.flags = 0;
  create_info.surface = mSurface;
  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  VulkanRendererAPI::QueryFamilyIndices indices = query_queue_families(mPhysicalDevice);
  std::array<std::uint32_t, 2> queue_family_indices = {indices.graphicsFamily.value(), indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
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

  VkResult result = vkCreateSwapchainKHR(sDevice, &create_info, nullptr, &sSwapChain);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create swap chain - {0}", string_VkResult(result))
  }

  vkGetSwapchainImagesKHR(sDevice, sSwapChain, &image_count, nullptr);
  mSwapChainImages.resize(image_count);
  vkGetSwapchainImagesKHR(sDevice, sSwapChain, &image_count, mSwapChainImages.data());

  mSwapChainFormat = surface_format.format;
  mSwapChainExtent = extent;
}

void VulkanRendererAPI::create_image_views() {
  mSwapChainImageViews.resize(mSwapChainImages.size());

  for (size_t idx = 0; idx < mSwapChainImages.size(); idx++) {
    VkImageViewCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.image = mSwapChainImages[idx];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = mSwapChainFormat;

    create_info.components.r = create_info.components.g = create_info.components.b = create_info.components.a =
        VK_COMPONENT_SWIZZLE_IDENTITY;

    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    VkResult result = vkCreateImageView(sDevice, &create_info, nullptr, &mSwapChainImageViews[idx]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create image views - {0}", string_VkResult(result))
    }
  }
}

void VulkanRendererAPI::create_render_pass() {
  VkAttachmentDescription color_attachment;
  color_attachment.format = mSwapChainFormat;
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

  VkResult result = vkCreateRenderPass(sDevice, &render_pass_info, nullptr, &mRenderPass);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create render pass - {0}", string_VkResult(result))
  }
}

void VulkanRendererAPI::create_graphics_pipeline() {
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
  dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamic_state.pDynamicStates = dynamicStates.data();

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
  viewport.width = (float)mSwapChainExtent.width;
  viewport.height = (float)mSwapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // ======================== Scissor ========================
  VkRect2D scissor;
  scissor.offset = {0, 0};
  scissor.extent = mSwapChainExtent;

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

  VkResult result = vkCreatePipelineLayout(sDevice, &pipeline_layout_info, nullptr, &mPipelineLayout);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create pipeline layout - {0}", string_VkResult(result))
  }

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = static_cast<std::uint32_t>(sShaderStages.size());
  pipeline_info.pStages = sShaderStages.data();
  pipeline_info.pVertexInputState = &vertex_input_state;
  pipeline_info.pInputAssemblyState = &input_assembly_state;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterizer;
  pipeline_info.pMultisampleState = &multisample_state_info;
  pipeline_info.pDepthStencilState = nullptr;
  pipeline_info.pColorBlendState = &color_blending;
  pipeline_info.pDynamicState = &dynamic_state;

  pipeline_info.layout = mPipelineLayout;
  pipeline_info.renderPass = mRenderPass;
  pipeline_info.subpass = 0;

  pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_info.basePipelineIndex = -1;

  result = vkCreateGraphicsPipelines(sDevice, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &sGraphicsPipeline);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create graphics pipeline - {0}", string_VkResult(result))
  }
}

void VulkanRendererAPI::create_frame_buffers() {
  mSwapChainFrameBuffers.resize(mSwapChainImages.size());

  for (size_t i = 0; i < mSwapChainImageViews.size(); i++) {
    std::array<VkImageView, 1> attachments = {mSwapChainImageViews[i]};

    VkFramebufferCreateInfo frame_buffer_info;
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.flags = 0;
    frame_buffer_info.pNext = nullptr;
    frame_buffer_info.renderPass = mRenderPass;
    frame_buffer_info.attachmentCount = attachments.size();
    frame_buffer_info.pAttachments = attachments.data();
    frame_buffer_info.width = mSwapChainExtent.width;
    frame_buffer_info.height = mSwapChainExtent.height;
    frame_buffer_info.layers = 1;

    VkResult result = vkCreateFramebuffer(sDevice, &frame_buffer_info, nullptr, &mSwapChainFrameBuffers[i]);
    if (result != VK_SUCCESS) {
      ARIA_CORE_ERROR("Failed to create frame buffer - {0}", string_VkResult(result))
    }
  }
}

void VulkanRendererAPI::create_command_pool() {
  QueryFamilyIndices queue_family_indicies = query_queue_families(mPhysicalDevice);

  VkCommandPoolCreateInfo cmd_pool_info;
  cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmd_pool_info.flags = 0;
  cmd_pool_info.pNext = nullptr;
  cmd_pool_info.queueFamilyIndex = queue_family_indicies.graphicsFamily.value();

  VkResult result = vkCreateCommandPool(sDevice, &cmd_pool_info, nullptr, &sCommandPool);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command pool - {0}", string_VkResult(result))
  }
}

VkCommandBuffer VulkanRendererAPI::create_vk_command_buffer() {
  VkCommandBufferAllocateInfo buffer_alloc_info;
  buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_alloc_info.pNext = nullptr;
  buffer_alloc_info.commandPool = sCommandPool;
  buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_alloc_info.commandBufferCount = 1;

  VkResult result = vkAllocateCommandBuffers(sDevice, &buffer_alloc_info, &sCommandBuffer);
  if (result != VK_SUCCESS) {
    ARIA_CORE_ERROR("Failed to create command buffer - {0}", string_VkResult(result))
    return nullptr;
  }
  return sCommandBuffer;
}

bool VulkanRendererAPI::has_validation_support() const {
  // how many instance layers can the vulkan system support?
  std::uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : mValidationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }
  return true;
}

void VulkanRendererAPI::populate_debug_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info) const {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = vulkan_log_callback;
}

VkResult VulkanRendererAPI::create_debug_util_messenger_ext(VkInstance instance,
                                                            const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
                                                            const VkAllocationCallbacks* p_allocator,
                                                            VkDebugUtilsMessengerEXT* p_debug_messenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, p_create_info, p_allocator, p_debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRendererAPI::vulkan_log_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data) {
  switch (message_severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      ARIA_CORE_INFO("Vulkan {0} Info: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      ARIA_CORE_TRACE("Vulkan {0} Trace: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      ARIA_CORE_ERROR("Vulkan {0} Error: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      ARIA_CORE_WARN("Vulkan {0} Error: {1}", get_message_type(message_type), p_callback_data->pMessage)
      break;
    default:
      ARIA_CORE_ASSERT(false, "Unknown error type")
      break;
  }
  // per vulkan 1.3 spec, pg.3566
  // The callback returns a VkBool32, which is interpreted in a layer-specified manner. The application
  // should always return VK_FALSE. The VK_TRUE value is reserved for use in layer development.
  return VK_FALSE;
}

std::string VulkanRendererAPI::get_message_type(VkDebugUtilsMessageTypeFlagsEXT message_type) {
  switch (message_type) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
      return "General";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
      return "Performance";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
      return "Validation";
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
      return "Device Address";
    default:
      return "Unknown Message Type";
  }
}

bool VulkanRendererAPI::is_suitable_vulkan_device(VkPhysicalDevice device) {
  QueryFamilyIndices queue_families = query_queue_families(device);

  bool swap_chain_supported = false;
  if (check_device_extensions_support(device)) {
    SwapChainDetails details = query_swap_chain_support(device);
    swap_chain_supported = !details.formats.empty() && !details.presentModes.empty();
  }

  return queue_families.is_complete() && swap_chain_supported;

  // VkPhysicalDeviceProperties properties;
  // vkGetPhysicalDeviceProperties(device, &properties);

  // VkPhysicalDeviceFeatures features;
  // vkGetPhysicalDeviceFeatures(device, &features);

  // as mentioned, can use scoring system to defer using Discrete GPU first
  // then integrated, or allow user to select
  // return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

std::string VulkanRendererAPI::get_vendor_name(std::uint32_t vendor_id) const {
  switch (vendor_id) {
    case 0x10DE:
      return "NVIDIA";
    case 0x1002:
      return "AMD";
    case 0x8086:
      return "INTEL";
    default:
      return "Unknown";
  }
}

VulkanRendererAPI::QueryFamilyIndices VulkanRendererAPI::query_queue_families(VkPhysicalDevice device) {
  QueryFamilyIndices indices;

  std::uint32_t query_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> query_families(query_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &query_family_count, query_families.data());

  int i = 0;
  // All possible queue families
  // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html

  for (const auto& queueFamily : query_families) {
    VkBool32 surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &surface_supported);

    if (surface_supported) {
      indices.presentFamily = i;
    }

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    if (indices.is_complete()) {
      break;
    }
    i++;
  }
  return indices;
}

VulkanRendererAPI::SwapChainDetails VulkanRendererAPI::query_swap_chain_support(VkPhysicalDevice device) {
  SwapChainDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);

  std::uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
  if (formatCount) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, details.formats.data());
  }

  std::uint32_t presentCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentCount, nullptr);
  if (presentCount) {
    details.presentModes.resize(presentCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentCount, details.presentModes.data());
  }

  return details;
}

std::vector<const char*> VulkanRendererAPI::get_glfw_required_extensions() {
  std::uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enable_validation_layers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool VulkanRendererAPI::check_device_extensions_support(VkPhysicalDevice device) {
  std::uint32_t count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

  std::vector<VkExtensionProperties> extensions(count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());

  std::set<std::string> required_extensions(mDeviceExtensions.begin(), mDeviceExtensions.end());

  for (const auto& extension : extensions) {
    required_extensions.erase(extension.extensionName);
  }
  return required_extensions.empty();
}

// By default, use VK_FORMAT_B8G8R8A8_SRGB and sRGB color space
VkSurfaceFormatKHR VulkanRendererAPI::get_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& formats) {
  for (const auto& format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return formats[0];
}

VkPresentModeKHR VulkanRendererAPI::get_present_mode(const std::vector<VkPresentModeKHR>& modes) {
  for (const auto& mode : modes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return mode;
    }
  }
  // Only guaranteed this mode
  // vulkan spec v1.3 pg 2717
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRendererAPI::get_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    auto* glfw_window = (GLFWwindow*)Application::get().get_window().get_native_window();
    int width;
    int height;

    glfwGetFramebufferSize(glfw_window, &width, &height);

    VkExtent2D extent = {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};

    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return extent;
  }
}

void VulkanRendererAPI::cleanup() {
  vkDestroyCommandPool(sDevice, sCommandPool, nullptr);

  for (auto buffer : mSwapChainFrameBuffers) {
    vkDestroyFramebuffer(sDevice, buffer, nullptr);
  }
  vkDestroyPipeline(sDevice, mGraphicsPipeline, nullptr);
  vkDestroyPipelineLayout(sDevice, mPipelineLayout, nullptr);
  vkDestroyRenderPass(sDevice, mRenderPass, nullptr);
  for (auto view : mSwapChainImageViews) {
    vkDestroyImageView(sDevice, view, nullptr);
  }
  vkDestroySwapchainKHR(sDevice, sSwapChain, nullptr);
  vkDestroyDevice(sDevice, nullptr);

  if (enable_validation_layers) {
    auto func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(sInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(sInstance, mDebugMessenger, nullptr);
    }
  }

  vkDestroySurfaceKHR(sInstance, mSurface, nullptr);
  vkDestroyInstance(sInstance, nullptr);
}

}  // namespace ARIA

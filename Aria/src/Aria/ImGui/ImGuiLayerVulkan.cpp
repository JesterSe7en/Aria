#include "ariapch.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Log.h"
#include "ImGuiLayerVulkan.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanLib.h"
#include "Platform/Vulkan/VulkanRendererApi.h"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace aria {

ImGuiLayerVulkan::ImGuiLayerVulkan() : ImGuiLayer("ImGuiLayerVulkan") {}

void ImGuiLayerVulkan::OnAttach() {
  // Load ImGui

  ARIA_CORE_ASSERT(glfwVulkanSupported(), "GLFW: Vulkan is not supported!")

  bool success = IMGUI_CHECKVERSION();
  ARIA_CORE_ASSERT(success, "Failed to initialize Dear ImGui")
  ARIA_CORE_INFO("Loaded Dear ImGui {0}", IMGUI_VERSION)

  Application &app = Application::Get();

  auto *renderer_api = dynamic_cast<VulkanRendererApi *>(app.GetRendererApi().get());
  Ref<VulkanDeviceManager> device_manager = renderer_api->GetVulkanDeviceManager();

  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;                                           // to suppress compiler warnings
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());

  ImGui::StyleColorsDark();

  auto *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForVulkan(window, true);

  ImGui_ImplVulkan_InitInfo init_info = {};
  Ref<VulkanInstance> vulkan_instance = renderer_api->GetVulkanInstance();
  init_info.Instance = renderer_api->GetVulkanInstance()->GetVKBInstance().instance;
  init_info.PhysicalDevice = device_manager->GetPhysicalDevice();
  init_info.Device = device_manager->GetLogicalDevice();
  init_info.QueueFamily = device_manager->GetQueueFamilyIndex();
  init_info.Queue = renderer_api->GetGraphicsQueue();
  init_info.PipelineCache = nullptr;
  init_info.Subpass = 0;
  init_info.MinImageCount = 2;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = nullptr;
  init_info.CheckVkResultFn = [](VkResult result) {
    // FIXME: This doesn't work fix it
    // if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Vulkan Error: {0}", result); }
  };

  // From DearImgui documentation in imgui_impl_vulkan.h...
  // [Configuration] in order to use a custom Vulkan function loader:
  // (1) You'll need to disable default Vulkan function prototypes.
  //     - As a compilation flag in your build system - DONE
  // (2) Call ImGui_ImplVulkan_LoadFunctions() before ImGui_ImplVulkan_Init() with your custom function.

  // You can use the default Vulkan loader using:
  //      ImGui_ImplVulkan_LoadFunctions([](const char* function_name, void*) { return vkGetInstanceProcAddr(your_vk_isntance, function_name); });

  ImGui_ImplVulkan_LoadFunctions([](const char *function_name, void *user_data) {
    auto vk_lib = VulkanLib::GetInstance();
    return vk_lib.LoadFunc(function_name, user_data);
    // this causes vulkanlib to be destroyed thus unloading vulkan-lib.  Casuing loading functions to fail
  });

  ImGui_ImplVulkan_Init(&init_info,
                        renderer_api->GetVulkanGraphicsPipeline()->GetVulkanRenderPass()->GetVkRenderPass());

  // init_info.PhysicalDevice = VulkanDeviceManager::GetInstance().GetPhysicalDevice();
  // init_info.Device = VulkanDeviceManager::GetInstance().GetLogicalDevice();
  // init_info.QueueFamily = VulkanDeviceManager::GetInstance().GetQueueFamilyIndex();
  // init_info.Queue = VulkanRendererApi::GetInstance().GetGraphicsQueue();
  // init_info.PipelineCache = nullptr;
  // init_info.Subpass = 0;
  // init_info.MinImageCount = 2;
  // init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  // init_info.Allocator = nullptr;
  // init_info.CheckVkResultFn = [](VkResult result) {
  //   if (result != VK_SUCCESS) { ARIA_CORE_ERROR("Vulkan Error: {0}", result); }
  // };

  // ImGui_ImplVulkan_Init(&init_info, VulkanGraphicsPipeline::GetInstance().GetRenderPass()->GetRenderPass());
}

void ImGuiLayerVulkan::OnDetach() {
  // ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayerVulkan::OnEvent(Event & /* event */) {}

void ImGuiLayerVulkan::OnUpdate(Timestep /* delta_time */) {}

void ImGuiLayerVulkan::OnImGuiRender() {}

void ImGuiLayerVulkan::Begin() const {
  // ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayerVulkan::End() const {
  static bool show_ = true;
  ImGui::ShowDemoWindow(&show_);
  ImGui::Render();

  // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VulkanRendererApi::GetInstance().GetCommandBuffers()[0]);

  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we
  // save/restore it to make it easier to paste this code elsewhere.

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *p_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(p_current_context);
  }
}
}// namespace aria

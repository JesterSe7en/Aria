#include "ariapch.h"
#include "ImGuiLayer.h"

#include "Aria/Core/Log.h"
#include "Aria/Core/Application.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace ARIA {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::on_attach() {
  // Load ImGui
  bool success = IMGUI_CHECKVERSION();
  ARIA_CORE_ASSERT(success, "Failed to initialize Dear ImGui")
  ARIA_CORE_INFO("Loaded Dear ImGui {0}", IMGUI_VERSION)

  Application& app = Application::get();

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;                                              // to suppress compiler warnings
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  io.DisplaySize = ImVec2((float)app.get_window().get_width(), (float)app.get_window().get_height());

  ImGui::StyleColorsDark();

  GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().get_native_window());

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void ImGuiLayer::on_detach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::on_event(Event& event) {}

void ImGuiLayer::begin() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end() const {
  static bool show = true;
  ImGui::ShowDemoWindow(&show);
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional Platform Windows
  // (Platform functions may change the current OpenGL context, so we
  // save/restore it to make it easier to paste this code elsewhere.

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}
}  // namespace ARIA
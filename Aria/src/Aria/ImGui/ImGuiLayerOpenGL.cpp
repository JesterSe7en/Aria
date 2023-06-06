#include "ariapch.h"

#include "Aria/Core/Application.h"
#include "Aria/Core/Log.h"
#include "ImGuiLayerOpenGL.h"

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

namespace aria {

ImGuiLayerOpenGL::ImGuiLayerOpenGL() : ImGuiLayer("ImGuiLayerOpenGL") {}

void ImGuiLayerOpenGL::OnAttach() {
  // Load ImGui
  bool success = IMGUI_CHECKVERSION();
  ARIA_CORE_ASSERT(success, "Failed to initialize Dear ImGui")
  ARIA_CORE_INFO("Loaded Dear ImGui {0}", IMGUI_VERSION)

  Application &app = Application::Get();

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
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version_);
}

void ImGuiLayerOpenGL::OnDetach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayerOpenGL::OnEvent(Event &event) {}

void ImGuiLayerOpenGL::OnUpdate(Timestep delta_time) {}

void ImGuiLayerOpenGL::OnImGuiRender() {}

void ImGuiLayerOpenGL::Begin() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayerOpenGL::End() const {
  static bool show_ = true;
  ImGui::ShowDemoWindow(&show_);
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
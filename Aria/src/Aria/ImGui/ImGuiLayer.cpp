#include "ariapch.h"
#include "ImGuiLayer.h"

#include <Aria/Log.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


#include "Aria/Application.h"

namespace Aria {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
  // Load ImGui
  bool success = IMGUI_CHECKVERSION();
  ARIA_CORE_ASSERT(success, "Failed to initialize Dear ImGui");
  ARIA_CORE_INFO("Loaded Dear ImGui {0}", IMGUI_VERSION);

  Application& app = Application::Get();

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io; // to suppress compiler warnings
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(),
                          (float)app.GetWindow().GetHeight());
  ImGui::StyleColorsDark();

  GLFWwindow* window =
      static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void ImGuiLayer::OnDetach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  static bool show = true;
  ImGui::ShowDemoWindow(&show);
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {}
}  // namespace Aria
#include "ariapch.h"
#include "Application.h"
#include "Aria/Core/Timestep.h"
#include "Aria/Events/ApplicationEvent.h"
#include "Aria/ImGui/ImGuiLayer.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/RendererApi.h"
#include <GLFW/glfw3.h>//TODO: abstract it out only delta time use this

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif// def WIN32

namespace aria {

Application *Application::p_application_ = nullptr;

// ortho params are actually what is given to us to use by default from OpenGL
Application::Application(ApplicationProps &props) {
  ARIA_CORE_ASSERT(!p_application_, "Application already exists.")
  p_application_ = this;

  RendererApi::SetApi(props.api);

  switch (props.api) {
    case RendererApi::Api::VULKAN:
      InitVulkanApp();
      break;
    case RendererApi::Api::OPEN_GL:
      InitOpenGlApp();
      break;
    default:
      ARIA_CORE_ASSERT(false, "Aria engine currently only supports Vulkan and OpenGL")
      break;
  }
}

void Application::InitVulkanApp() {
  p_window_ = std::unique_ptr<Window>(Window::Create());
  p_window_->SetVSync(false);
  p_window_->SetEventCallback(ARIA_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  // TODO: still need to fix imgui for vulkan.  More involved
  //  p_im_gui_layer_ = ImGuiLayer::Create();
  //  PushOverlay(p_im_gui_layer_.get());
}

void Application::InitOpenGlApp() {
  p_window_ = std::unique_ptr<Window>(Window::Create());
  p_window_->SetVSync(false);
  p_window_->SetEventCallback(ARIA_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  p_im_gui_layer_ = ImGuiLayer::Create();
  PushOverlay(p_im_gui_layer_.get());
}

void Application::Run() {
  while (running_) {
    auto time = (float) glfwGetTime();// Platform::GetTime() should be used.  Somehow grab the time passed from the
    // OS. Windows as QueryPerformanceTimer()
    Timestep delta_time = time - last_frame_time_;
    last_frame_time_ = time;

    for (Layer *layer : layer_stack_) { layer->OnUpdate(delta_time); }

    auto api_used = RendererApi::GetApi();
    if (api_used == RendererApi::Api::VULKAN) {
      p_window_->OnUpdate();
    } else {
      p_im_gui_layer_->Begin();
      for (Layer *layer : layer_stack_) { layer->OnImGuiRender(); }
      p_im_gui_layer_->End();

      p_window_->OnUpdate();
    }
  }
}

void Application::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(ARIA_BIND_EVENT_FN(Application::OnWindowClose));

  // Go through the Layer Stack (backwards) and fire off events

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->OnEvent(e);
    if (e.handled_) {
      // stop at the first ("highest z value") layer (or overlay) that responded
      // to the event firing
      break;
    }
  }
}

void Application::PushLayer(Layer *layer) {
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer *overlay) {
  layer_stack_.PushOverlay(overlay);
  overlay->OnAttach();
}

void Application::PopLayer(Layer *layer) {
  layer_stack_.PopLayer(layer);
  layer->OnDetach();
}

void Application::PopOverlay(Layer *overlay) {
  layer_stack_.PopLayer(overlay);
  overlay->OnDetach();
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
  running_ = false;
  return true;
}
}// namespace aria

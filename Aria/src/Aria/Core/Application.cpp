#include "ariapch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/Camera.h"
#include "Aria/Core/Timestep.h"

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  // def WIN32

namespace aria {

Application *Application::p_application_ = nullptr;

// ortho params are actually what is given to us to use by defualt from OpenGL
Application::Application() {
  ARIA_CORE_ASSERT(!p_application_, "Application already exists.")
  p_application_ = this;

  window_ = std::unique_ptr<Window>(Window::Create());
  window_->SetVSync(false);
  window_->SetEventCallback(ARIA_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  im_gui_layer_ = new ImGuiLayer();
  PushOverlay(im_gui_layer_);
}

void Application::Run() {
  while (running_) {
    float time = (float)glfwGetTime();  // Platform::GetTime() should be used.  Somehow grab the time passed from the
                                        // OS. Windows as QueryPerformaceTimer()
    Timestep timestep = time - last_frame_time_;
    last_frame_time_ = time;

    for (Layer *layer : layer_stack_) {
      layer->OnUpdate(timestep);
    }

    im_gui_layer_->Begin();
    for (Layer *layer : layer_stack_) {
      layer->OnImGuiRender();
    }
    im_gui_layer_->End();

    window_->OnUpdate();
  }
}
void Application::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(ARIA_BIND_EVENT_FN(Application::OnWindowClose));

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
}  // namespace ARIA

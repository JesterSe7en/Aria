#include "ariapch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Log.h"

namespace Aria {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
  ARIA_CORE_ASSERT(!s_Instance, "Application already exists.");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application() {}

void Application::Run() {
  while (m_Running) {
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Layer* layer : m_LayerStack) {
      layer->OnUpdate();
    }

    m_Window->OnUpdate();
  }
}
void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(
      BIND_EVENT_FN(Application::OnWindowClose));

  ARIA_CORE_TRACE("{0}", e);

  // Go through the Layer Stack (backwards) and fire off events

  for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
    (*--it)->OnEvent(e);
    if (e.Handled) {
      // stop at the first ("highest z value") layer (or overlay) that responded
      // to the event firing
      break;
    }
  }
}
void Application::PushLayer(Layer* layer) { 
  m_LayerStack.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
  m_LayerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

void Application::PopLayer(Layer* layer) {
  m_LayerStack.PopLayer(layer);
  layer->OnDetach();
}

void Application::PopOverlay(Layer* overlay) {
  m_LayerStack.PopLayer(overlay);
  overlay->OnDetach();
}

//TODO: add remove/disable layer (needs to call OnDetatch as well)

bool Application::OnWindowClose(WindowCloseEvent& e) {
  m_Running = false;
  return true;
}
}  // namespace Aria

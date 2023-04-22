#include "ariapch.h"
#include "Application.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Input.h"
#include "Aria/Log.h"

#include "Aria/Renderer/Buffer.h"

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  // def WIN32

namespace Aria {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
  ARIA_CORE_ASSERT(!s_Instance, "Application already exists.");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);

  float vertices[3 * 3] = {
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
  };
  uint32_t indicies[3] = {0, 1, 2};

  vertex_buffer_.reset(VertexBuffer::Create(vertices, sizeof(vertices))); 

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

  index_buffer_.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));

  // Why doesn't this accept relative path?
  shader_.reset(new Shader("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basic.shader"));
}

Application::~Application() {}

void Application::Run() {
  while (m_Running) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_->Bind();
    glBindVertexArray(vertex_array_);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   nullptr);

    for (Layer* layer : m_LayerStack) {
      layer->OnUpdate();
    }

    // this differs from the event mouse position
    // this one gives absolute position of the mouse outside of glfwWindow
    auto [x, y] = Input::GetMousePosition();
    //ARIA_CORE_TRACE("{0}, {1}", x, y);


    m_Window->OnUpdate();
  }
}
void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.Dispatch<WindowCloseEvent>(
      BIND_EVENT_FN(Application::OnWindowClose));

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

bool Application::OnWindowClose(WindowCloseEvent& e) {
  m_Running = false;
  return true;
}
}  // namespace Aria

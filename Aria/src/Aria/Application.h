#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Renderer/Buffer.h"
#include "Aria/Renderer/Shader.h"

namespace Aria {

class ARIA_API Application {
 public:
  Application();
  virtual ~Application();

  void Run();
  void OnEvent(Event& e);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  inline Window& GetWindow() { return *m_Window; }

  inline static Application& Get() { return *s_Instance; }

 private:
  bool OnWindowClose(WindowCloseEvent& e);

  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
  LayerStack m_LayerStack;

 private:
  static Application* s_Instance;
  unsigned int vertex_array_;

  std::unique_ptr<Shader> shader_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<IndexBuffer> index_buffer_;
};

// To be defined in CLIENT
Application* CreateApplication();
}  // namespace Aria
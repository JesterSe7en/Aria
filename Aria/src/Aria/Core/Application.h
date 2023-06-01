#pragma once

#include "Aria/Core/Application.h"
#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/Event.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"
#include "Aria/ImGui/ImGuiLayer.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/RendererApi.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "Base.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"

namespace aria {

struct ARIA_API ApplicationProps {
  RendererApi::Api api;
};

class ARIA_API Application {
 public:
  Application(ApplicationProps &props);
  virtual ~Application() = default;

  void Run();
  void OnEvent(Event &e);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *overlay);

  inline Window &GetWindow() { return *p_window_; }

  inline static Application &Get() { return *p_application_; }

 private:
  bool OnWindowClose(WindowCloseEvent &e);

  void InitVulkanApp();
  void InitOpenGlApp();

  std::unique_ptr<Window> p_window_;
  bool running_ = true;
  LayerStack layer_stack_;

  Timestep timestep_;
  float last_frame_time_ = 0.0f;

 private:
  static Application *p_application_;
  ImGuiLayer *p_im_gui_layer_;
};

// To be defined in CLIENT
Application *CreateApplication();
}// namespace aria

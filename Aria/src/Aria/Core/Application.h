#pragma once

#include "Base.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Aria/ImGui/ImGuiLayer.h"
#include "Aria/Events/Event.h"
#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Events/MouseEvent.h"
#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "Window.h"

namespace aria {

class ARIA_API Application {
 public:
  Application();
  virtual ~Application() = default;

  void Run();
  void OnEvent(Event &e);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);
  void PopLayer(Layer *layer);
  void PopOverlay(Layer *overlay);

  inline Window &GetWindow() { return *window_; }

  inline static Application &Get() { return *p_application_; }

 private:
  bool OnWindowClose(WindowCloseEvent &e);

  std::unique_ptr<Window> window_;
  bool running_ = true;
  LayerStack layer_stack_;

  Timestep timestep_;
  float last_frame_time_ = 0.0f;

 private:
  static Application *p_application_;
  ImGuiLayer *im_gui_layer_;
};

// To be defined in CLIENT
Application *CreateApplication();
}  // namespace ARIA

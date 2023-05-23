#pragma once

#include "Aria/Core/Application.hpp"
#include "Base.hpp"
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Aria/ImGui/ImGuiLayer.hpp"
#include "Aria/Events/Event.hpp"
#include "Aria/Events/ApplicationEvent.hpp"
#include "Aria/Events/KeyEvent.hpp"
#include "Aria/Events/MouseEvent.hpp"
#include "Aria/Renderer/RendererApi.hpp"
#include "Aria/Renderer/Camera.hpp"
#include "Aria/Renderer/Buffer.hpp"
#include "Aria/Renderer/Shader.hpp"
#include "Aria/Renderer/VertexArray.hpp"
#include "Window.hpp"

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
}  // namespace ARIA

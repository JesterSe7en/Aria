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

namespace ARIA {

class ARIA_API Application {
 public:
  Application();
  virtual ~Application() = default;

  void run();
  void on_event(Event &e);

  void push_layer(Layer *layer);
  void push_overlay(Layer *overlay);
  void pop_layer(Layer *layer);
  void pop_overlay(Layer *overlay);

  inline Window &get_window() { return *mWindow; }

  inline static Application &get() { return *sInstance; }

 private:
  bool on_window_close(WindowCloseEvent &e);

  std::unique_ptr<Window> mWindow;
  bool mRunning = true;
  LayerStack mLayerStack;

  Timestep mTimestep;
  float mLastFrameTime = 0.0f;

 private:
  static Application *sInstance;
  ImGuiLayer *mImGuiLayer;
};

// To be defined in CLIENT
Application *create_application();
}  // namespace ARIA

#pragma once

#include "Renderer/Camera.h"
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Window.h"

namespace ARIA {

class ARIA_API Application {
 public:
  Application();
  virtual ~Application();

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
};

// To be defined in CLIENT
Application *create_application();
}  // namespace ARIA

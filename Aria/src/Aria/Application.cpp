#include "ariapch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Input.h"
#include "Aria/Log.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/VertexArray.h"
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/Camera.h"

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  // def WIN32

namespace ARIA {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application *Application::sInstance = nullptr;

// ortho params are actually what is given to us to use by defualt from OpenGL
Application::Application() {
  ARIA_CORE_ASSERT(!sInstance, "Application already exists.");
  sInstance = this;

  mWindow = std::unique_ptr<Window>(Window::create());
  mWindow->set_event_callback(BIND_EVENT_FN(on_event));
}

Application::~Application() {}

void Application::run() {
  while (mRunning) {
    for (Layer *layer : mLayerStack) {
      layer->on_update();
    }

    mWindow->on_update();
  }
}
void Application::on_event(Event &e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::on_window_close));

  // Go through the Layer Stack (backwards) and fire off events

  for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
    (*--it)->on_event(e);
    if (e.Handled) {
      // stop at the first ("highest z value") layer (or overlay) that responded
      // to the event firing
      break;
    }
  }
}
void Application::push_layer(Layer *layer) {
  mLayerStack.push_layer(layer);
  layer->on_attach();
}

void Application::push_overlay(Layer *overlay) {
  mLayerStack.push_overlay(overlay);
  overlay->on_attach();
}

void Application::pop_layer(Layer *layer) {
  mLayerStack.pop_layer(layer);
  layer->on_detach();
}

void Application::pop_overlay(Layer *overlay) {
  mLayerStack.pop_layer(overlay);
  overlay->on_detach();
}

bool Application::on_window_close(WindowCloseEvent &e) {
  mRunning = false;
  return true;
}
}  // namespace ARIA

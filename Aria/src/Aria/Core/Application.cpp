#include "ariapch.h"

#include "Application.h"

#include "Aria/Core/Timestep.h"
#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/Renderer.h"

#include <GLFW/glfw3.h>  //TODO: abstract it out only delta time use this

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  // def WIN32

namespace ARIA {

Application *Application::sInstance = nullptr;

// ortho params are actually what is given to us to use by default from OpenGL
Application::Application(ApplicationProps &props) {
  ARIA_CORE_ASSERT(!sInstance, "Application already exists.")
  sInstance = this;

  RendererAPI::set_api(props.mRendererAPI);

  switch (props.mRendererAPI) {
    case RendererAPI::API::Vulkan:
      init_vulkan_app();
      break;
    case RendererAPI::API::OpenGL:
      init_opengl_app();
      break;
    default:
      ARIA_CORE_ASSERT(false, "Aria engine currently only supports Vulkan and OpenGL")
  }
}

void Application::init_vulkan_app() {
  // TODO: ordering for creating window and initializing renderer is different than opengl for vulkan
  mWindow = std::unique_ptr<Window>(Window::create());
  mWindow->set_vsync(false);
  mWindow->set_event_callback(ARIA_BIND_EVENT_FN(Application::on_event));
  Renderer::init();

  // TODO: this imgui uses opengl to load
  //  mImGuiLayer = new ImGuiLayer();
  //  push_overlay(mImGuiLayer);
}

void Application::init_opengl_app() {
  mWindow = std::unique_ptr<Window>(Window::create());
  mWindow->set_vsync(false);
  mWindow->set_event_callback(ARIA_BIND_EVENT_FN(Application::on_event));

  Renderer::init();

  mImGuiLayer = new ImGuiLayer();
  push_overlay(mImGuiLayer);
}

void Application::run() {
  while (mRunning) {
    auto time = (float)glfwGetTime();  // Platform::GetTime() should be used.  Somehow grab the time passed from the
                                       // OS. Windows as QueryPerformaceTimer()
    Timestep delta_time = time - mLastFrameTime;
    mLastFrameTime = time;

    for (Layer *layer : mLayerStack) {
      layer->on_update(delta_time);
    }

    // mImGuiLayer->begin();
    // for (Layer *layer : mLayerStack) {
    //   layer->on_imgui_render();
    // }
    // mImGuiLayer->end();

    mWindow->on_update();
  }
}
void Application::on_event(Event &e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(ARIA_BIND_EVENT_FN(Application::on_window_close));

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

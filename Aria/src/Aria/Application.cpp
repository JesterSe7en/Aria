#include "ariapch.h"
#include "Application.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Input.h"
#include "Aria/Log.h"

#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/VertexArray.h"

#ifdef WIN32
#include <Windows.h>
extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif  // def WIN32

namespace ARIA {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::sInstance = nullptr;

Application::Application() {
  ARIA_CORE_ASSERT(!sInstance, "Application already exists.");
  sInstance = this;

  mWindow = std::unique_ptr<Window>(Window::create());
  mWindow->set_event_callback(BIND_EVENT_FN(on_event));

  //glGenVertexArrays(1, &mVertex_Array);
  //glBindVertexArray(mVertex_Array);

  mVertex_Array.reset(VertexArray::create());
  mVertex_Array->bind();

  float vertices[3 * 3] = {
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
  };
  uint32_t indicies[3] = {0, 1, 2};


  auto buffer = VertexBuffer::create(vertices, sizeof(vertices));
  BufferElement pos(ShaderPrimitiveType::Float, "position", false);
  const BufferLayout layout({pos});
  buffer->set_layout(layout);
  mVertex_Buffer.reset(buffer); 

  
   
  //glEnableVertexAttribArray(0);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

  mIndex_Buffer.reset(IndexBuffer::create(indicies, sizeof(indicies) / sizeof(uint32_t)));

  mVertex_Array->add_vertex_buffer(*mVertex_Buffer);
  mVertex_Array->set_index_buffer(*mIndex_Buffer);

  // Why doesn't this accept relative path?
  mShader.reset(new Shader("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basic.shader"));
}

Application::~Application() {}

void Application::run() {
  while (mRunning) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->bind();
    mVertex_Array->bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                   nullptr);

    for (Layer* layer : mLayer_Stack) {
      layer->on_update();
    }

    // this differs from the event mouse position
    // this one gives absolute position of the mouse outside of glfwWindow
    auto [x, y] = Input::get_mouse_position();
    //ARIA_CORE_TRACE("{0}, {1}", x, y);


    mWindow->on_update();
  }
}
void Application::on_event(Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(
      BIND_EVENT_FN(Application::on_window_close));

  // Go through the Layer Stack (backwards) and fire off events

  for (auto it = mLayer_Stack.end(); it != mLayer_Stack.begin();) {
    (*--it)->on_event(e);
    if (e.Handled) {
      // stop at the first ("highest z value") layer (or overlay) that responded
      // to the event firing
      break;
    }
  }
}
void Application::push_layer(Layer* layer) { 
  mLayer_Stack.push_layer(layer);
  layer->on_attach();
}

void Application::push_overlay(Layer* overlay) {
  mLayer_Stack.push_overlay(overlay);
  overlay->on_attach();
}

void Application::pop_layer(Layer* layer) {
  mLayer_Stack.pop_layer(layer);
  layer->on_detach();
}

void Application::pop_overlay(Layer* overlay) {
  mLayer_Stack.pop_layer(overlay);
  overlay->on_detach();
}

bool Application::on_window_close(WindowCloseEvent& e) {
  mRunning = false;
  return true;
}
}  // namespace ARIA

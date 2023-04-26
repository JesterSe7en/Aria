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

  // --------------- Rendering TRIANGLE ---------------
  mTriangleVA.reset(VertexArray::create());

  float triangleVertices[3 * 7] = {
      -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
      0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
  };
  mTriangleVB.reset(VertexBuffer::create(triangleVertices, sizeof(triangleVertices)));
  BufferLayout layout = {
    {ShaderPrimitiveType::Float3, "a_Position" },
    {ShaderPrimitiveType::Float4, "a_Color" }
  };
  mTriangleVB->set_layout(layout);
  mTriangleVA->add_vertex_buffer(mTriangleVB);

  //index buffer
  uint32_t triangleIndicies[3] = { 0, 1, 2 };
  mTriangleIB.reset(IndexBuffer::create(triangleIndicies, sizeof(triangleIndicies) / sizeof(uint32_t)));
  mTriangleVA->set_index_buffer(mTriangleIB);

    // Why doesn't this accept relative path?
  mTriangleShader.reset(new Shader(
      "C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicTriangle.shader"));

  // --------------- Rendering SQUARE ---------------
  mSquareVA.reset(VertexArray::create());

  float squareVertices[3 * 4] = {
    -0.75f, -0.75f, 0.0f,
    0.75f, -0.75f, 0.0f,
    0.75f, 0.75f, 0.0f,
    -0.75f, 0.75f, 0.0f
  };

  mSquareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));

  mSquareVB->set_layout({{ShaderPrimitiveType::Float3, "a_Position"}});
  mSquareVA->add_vertex_buffer(mSquareVB);

  uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

  mSquareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
  mSquareVA->set_index_buffer(mSquareIB);

  mSquareShader.reset(new Shader(
      "C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicSquare.shader"));
}

Application::~Application() {}

void Application::run() {
  while (mRunning) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //RenderCommand::SetClearColor()
    //RenderCommand::Clear()
    //
    //Renderer::BeginScene();

    //shader->bind();
    //Renderer::submit(vertexarray);

    //shader->bind();
    //Renderer::submit(vertexarray);


    //Renderer::EndScene();



    mSquareShader->bind();
    mSquareVA->bind();
    glDrawElements(GL_TRIANGLES, mSquareVA->get_index_buffer()->get_count(),
                   GL_UNSIGNED_INT,
                   nullptr);

    mTriangleShader->bind();
    mTriangleVA->bind();
    glDrawElements(GL_TRIANGLES, mTriangleVA->get_index_buffer()->get_count(),
                   GL_UNSIGNED_INT,
                   nullptr);

    for (Layer* layer : mLayerStack) {
      layer->on_update();
    }

    mWindow->on_update();
  }
}
void Application::on_event(Event& e) {
  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(
      BIND_EVENT_FN(Application::on_window_close));

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
void Application::push_layer(Layer* layer) { 
  mLayerStack.push_layer(layer);
  layer->on_attach();
}

void Application::push_overlay(Layer* overlay) {
  mLayerStack.push_overlay(overlay);
  overlay->on_attach();
}

void Application::pop_layer(Layer* layer) {
  mLayerStack.pop_layer(layer);
  layer->on_detach();
}

void Application::pop_overlay(Layer* overlay) {
  mLayerStack.pop_layer(overlay);
  overlay->on_detach();
}

bool Application::on_window_close(WindowCloseEvent& e) {
  mRunning = false;
  return true;
}
}  // namespace ARIA

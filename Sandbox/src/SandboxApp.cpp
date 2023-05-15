#include "Aria/Core/Application.h"
#include "Aria/Renderer/RendererAPI.h"
#include "ExampleLayer.h"

#include <Aria.h>
#include <Aria/Core/EntryPoint.h>

class Sandbox : public ARIA::Application {
 public:
  Sandbox(ARIA::ApplicationProps& props) : ARIA::Application(props) {
    auto example_layer = new ExampleLayer();
    push_layer(example_layer);
  };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() {
  ApplicationProps props = {RendererAPI::API::Vulkan};
  // ApplicationProps props = {RendererAPI::API::OpenGL};
  return new Sandbox(props);
}

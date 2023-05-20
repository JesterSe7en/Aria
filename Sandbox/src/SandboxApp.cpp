#include "Aria/Core/Application.h"
#include "Aria/Renderer/RendererApi.h"
#include "ExampleLayer.h"
#include "VulkanLayer.h"

#include <Aria.h>
#include <Aria/Core/EntryPoint.h>

class Sandbox : public aria::Application {
 public:
  Sandbox(aria::ApplicationProps &props) : aria::Application(props) {
    // auto example_layer = new ExampleLayer();
    // push_layer(example_layer);

    auto vulkan_layer = new VulkanLayer();
    PushLayer(vulkan_layer);
  };
  ~Sandbox() {};
};

aria::Application *aria::CreateApplication() {
  ApplicationProps props = {RendererApi::Api::VULKAN};
  // ApplicationProps props = {RendererAPI::API::OpenGL};
  return new Sandbox(props);
}

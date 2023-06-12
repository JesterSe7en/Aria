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
    // PushLayer(example_layer);

    auto vulkan_layer = new VulkanLayer();
    PushLayer(vulkan_layer);
  };
  ~Sandbox() override = default;
};

aria::Application *aria::CreateApplication() {
  ApplicationProps props = {RendererApi::Api::VULKAN};
  // ApplicationProps props = {RendererApi::Api::OPEN_GL};
  return new Sandbox(props);
}

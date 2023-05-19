#include <Aria.h>
#include <Aria/Core/EntryPoint.h>

#include "ExampleLayer.h"

class Sandbox : public aria::Application {
 public:
  Sandbox() {
    auto example_layer = new ExampleLayer();
    PushLayer(example_layer);
  };
  ~Sandbox(){};
};

aria::Application* aria::CreateApplication() { return new Sandbox(); }

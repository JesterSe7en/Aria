#include <Aria.h>
#include <Aria/EntryPoint.h>

#include "ExampleLayer.h"

class Sandbox : public ARIA::Application {
 public:
  Sandbox() {
    auto example_layer = new ExampleLayer();
    push_layer(example_layer);
  };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() { return new Sandbox(); }

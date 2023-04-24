#include <Aria.h>

class Sandbox : public ARIA::Application {
 public:
  Sandbox() { push_overlay(new ARIA::ImGuiLayer()); };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() { return new Sandbox(); }
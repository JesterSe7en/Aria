#include <Aria.h>

class InputLayer : public ARIA::Layer {
 public:
  InputLayer() : Layer("InputLayer") {}

  void on_update() override {
    if (ARIA::Input::is_key_pressed(ARIA_KEY_W)) {
      ARIA_TRACE("You hit the W key");
    }
  }

  void on_event(ARIA::Event& event) override {
    if (event.get_event_type() == ARIA::EventType::KeyPressed) {
      ARIA::KeyPressedEvent& e = (ARIA::KeyPressedEvent&)event;
      ARIA_TRACE("Keypressed event detected...");
    }
  }
};

class Sandbox : public ARIA::Application {
 public:
  Sandbox() {
    auto input_layer = new InputLayer();
    push_layer(input_layer);
    auto imgui = new ARIA::ImGuiLayer();
    push_overlay(imgui);
  };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() { return new Sandbox(); }
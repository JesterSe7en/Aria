#include <Aria.h>

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer() : Layer("ExampleLayer") {}

  void on_update() override { 
    //ARIA_INFO("ExampleLayer: Inside on_update"); 
  }

  void on_event(ARIA::Event& event) override {
    //ARIA_TRACE("ExampleLayer: {0}", event);
  }
};

class ExampleOverlay : public ARIA::Layer {
 public:
  ExampleOverlay() : Layer("ExampleOverlay") {}

  void on_update() override { 
    if (ARIA::Input::is_key_pressed(ARIA_KEY_A)) {
      ARIA_TRACE("A key is pressed!");
    }
    //ARIA_INFO("ExampleOverlay: Inside on_update"); 
  }

  void on_event(ARIA::Event& event) override {
    event.Handled = true;
    //ARIA_TRACE("ExampleOverlay: {0}", event);
  }
};

class Sandbox : public ARIA::Application {
 public:
  Sandbox() {
    push_overlay(new ARIA::ImGuiLayer());
    //push_layer(new ExampleLayer());
    push_overlay(new ExampleOverlay());
  };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() { return new Sandbox(); }
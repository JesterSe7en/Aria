#include <Aria.h>

class ExampleLayer : public Aria::Layer {
 public:
  ExampleLayer() : Layer("ExampleLayer") {}

  void OnUpdate() override { 
    //ARIA_INFO("ExampleLayer: Inside OnUpdate"); 
  }

  void OnEvent(Aria::Event& event) override {
    //ARIA_TRACE("ExampleLayer: {0}", event);
  }
};

class ExampleOverlay : public Aria::Layer {
 public:
  ExampleOverlay() : Layer("ExampleOverlay") {}

  void OnUpdate() override { 
    if (Aria::Input::IsKeyPressed(ARIA_KEY_A)) {
      ARIA_TRACE("A key is pressed!");
    }
    //ARIA_INFO("ExampleOverlay: Inside OnUpdate"); 
  }

  void OnEvent(Aria::Event& event) override {
    event.Handled = true;
    //ARIA_TRACE("ExampleOverlay: {0}", event);
  }
};

class Sandbox : public Aria::Application {
 public:
  Sandbox() {
    PushOverlay(new Aria::ImGuiLayer());
    //PushLayer(new ExampleLayer());
    PushOverlay(new ExampleOverlay());
  };
  ~Sandbox(){};
};

Aria::Application* Aria::CreateApplication() { return new Sandbox(); }
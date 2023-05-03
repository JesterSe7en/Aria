#include <Aria.h>

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer() : Layer("Example Layer") {}

  void on_update() override {
    ARIA::RenderCommand::set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ARIA::RenderCommand::clear();

    mOrthoCamera.set_position({0.5f, 0.5f, 0.0f});
    mOrthoCamera.set_rotation(45.0f);
    ARIA::Renderer::begin_scene(mOrthoCamera);

    ARIA::Renderer::submit(mSquareShader, mSquareVA);
    ARIA::Renderer::submit(mTriangleShader, mTriangleVA);

    ARIA::Renderer::end_scene();

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

 private:
  ARIA::OrthographicCamera mOrthoCamera = ARIA::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
  std::shared_ptr<ARIA::Shader> mTriangleShader;
  std::shared_ptr<ARIA::VertexBuffer> mTriangleVB;
  std::shared_ptr<ARIA::IndexBuffer> mTriangleIB;
  std::shared_ptr<ARIA::VertexArray> mTriangleVA;

  std::shared_ptr<ARIA::VertexArray> mSquareVA;
  std::shared_ptr<ARIA::VertexBuffer> mSquareVB;
  std::shared_ptr<ARIA::IndexBuffer> mSquareIB;
  std::shared_ptr<ARIA::Shader> mSquareShader;
};

class Sandbox : public ARIA::Application {
 public:
  Sandbox() {
    auto example_layer = new ExampleLayer();
    push_layer(example_layer);
    auto imgui = new ARIA::ImGuiLayer();
    push_overlay(imgui);
  };
  ~Sandbox(){};
};

ARIA::Application* ARIA::create_application() { return new Sandbox(); }
#include <Aria.h>

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer() : Layer("Example Layer") {
    // --------------- Rendering TRIANGLE ---------------
    mTriangleVA.reset(ARIA::VertexArray::create());

    // vertex buffer
    float triangleVertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
        0.3f,  0.8f,  1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f,
    };
    mTriangleVB.reset(ARIA::VertexBuffer::create(triangleVertices, sizeof(triangleVertices)));
    ARIA::BufferLayout layout = {{ARIA::ShaderPrimitiveType::Float3, "a_Position"},
                                 {ARIA::ShaderPrimitiveType::Float4, "a_Color"}};
    mTriangleVB->set_layout(layout);
    mTriangleVA->add_vertex_buffer(mTriangleVB);

    // index buffer
    uint32_t triangleIndicies[3] = {0, 1, 2};
    mTriangleIB.reset(ARIA::IndexBuffer::create(triangleIndicies, sizeof(triangleIndicies) / sizeof(uint32_t)));
    mTriangleVA->set_index_buffer(mTriangleIB);

    // Why doesn't this accept relative path?
    mTriangleShader.reset(new ARIA::Shader("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicTriangle.shader"));

    // --------------- Rendering SQUARE ---------------
    mSquareVA.reset(ARIA::VertexArray::create());

    float squareVertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.75f, 0.75f, 0.0f, -0.75f, 0.75f, 0.0f};

    mSquareVB.reset(ARIA::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

    mSquareVB->set_layout({{ARIA::ShaderPrimitiveType::Float3, "a_Position"}});
    mSquareVA->add_vertex_buffer(mSquareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

    mSquareIB.reset(ARIA::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    mSquareVA->set_index_buffer(mSquareIB);

    mSquareShader.reset(new ARIA::Shader("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicSquare.shader"));
  }

  void on_update() override {
    ARIA::RenderCommand::set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ARIA::RenderCommand::clear();

    mOrthoCamera.set_position(mPosition);
    // mOrthoCamera.set_rotation(45.0f);

    ARIA::Renderer::begin_scene(mOrthoCamera);

    ARIA::Renderer::submit(mSquareShader, mSquareVA);
    ARIA::Renderer::submit(mTriangleShader, mTriangleVA);

    ARIA::Renderer::end_scene();

    if (ARIA::Input::is_key_pressed(ARIA_KEY_W)) {
      mPosition.y -= 0.1f;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_A)) {
      mPosition.x += 0.1f;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_S)) {
      mPosition.y += 0.1f;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_D)) {
      mPosition.x -= 0.1f;
    }

    ARIA_TRACE("current position = {0}, {1}, {2}", mPosition.x, mPosition.y, mPosition.z);
  }

  void on_event(ARIA::Event& event) override {
    // if (event.get_event_type() == ARIA::EventType::KeyPressed) {
    //   ARIA::KeyPressedEvent& e = (ARIA::KeyPressedEvent&)event;
    //   ARIA_TRACE("Keypressed event detected...");
    // }
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

  glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
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
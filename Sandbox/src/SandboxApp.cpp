#include <Aria.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer() : Layer("Example Layer"), mSquarePosition(0.0f) {
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

    mOrthoCamera.set_position({0.0f, 0.0f, 0.0f});
  }

  void on_update(ARIA::Timestep delta_time) override {
    auto position = mOrthoCamera.get_position();
    auto rotation = mOrthoCamera.get_rotation();

    if (ARIA::Input::is_key_pressed(ARIA_KEY_W)) {
      position.y -= camera_move_speed * delta_time;
    } else if (ARIA::Input::is_key_pressed(ARIA_KEY_S)) {
      position.y += camera_move_speed * delta_time;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_A)) {
      position.x += camera_move_speed * delta_time;
    } else if (ARIA::Input::is_key_pressed(ARIA_KEY_D)) {
      position.x -= camera_move_speed * delta_time;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_R)) {
      mOrthoCamera.set_position({0.0f, 0.0f, 0.0f});
      mOrthoCamera.set_rotation(0.0f);
      return;
    }

    // ---------------------------------------------

    if (ARIA::Input::is_key_pressed(ARIA_KEY_J)) {
      mSquarePosition.x -= square_move_speed * delta_time;
    } else if (ARIA::Input::is_key_pressed(ARIA_KEY_L)) {
      mSquarePosition.x += square_move_speed * delta_time;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_I)) {
      mSquarePosition.y += square_move_speed * delta_time;
    } else if (ARIA::Input::is_key_pressed(ARIA_KEY_K)) {
      mSquarePosition.y -= square_move_speed * delta_time;
    }

    // ----------------------------------------------

    if (ARIA::Input::is_key_pressed(ARIA_KEY_LEFT)) {
      rotation += camera_rotate_speed * delta_time;
    }

    if (ARIA::Input::is_key_pressed(ARIA_KEY_RIGHT)) {
      rotation -= camera_rotate_speed * delta_time;
    }

    mOrthoCamera.set_position(position);
    mOrthoCamera.set_rotation(rotation);

    // delta_time with the float operator is returning time in SECONDS
    ARIA::RenderCommand::set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ARIA::RenderCommand::clear();

    ARIA::Renderer::begin_scene(mOrthoCamera);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), mSquarePosition);

    ARIA::Renderer::submit(mSquareShader, mSquareVA, transform);
    ARIA::Renderer::submit(mTriangleShader, mTriangleVA);

    ARIA::Renderer::end_scene();
  }

  void on_event(ARIA::Event& event) override {
    // ARIA::EventDispatcher dispatcher(event);
    // dispatcher.dispatch<ARIA::KeyPressedEvent>(ARIA_BIND_EVENT_FN(ExampleLayer::on_key_pressed_event));
  }

  // bool on_key_pressed_event(ARIA ::KeyPressedEvent& event) {
  // int key_code = event.get_key_code();
  // auto pos = mOrthoCamera.get_position();

  // we need to determine delta time to really move the camera independent of frame rate.
  // alternatively, use the old method of doing input polling on the on_update() func
  // switch (key_code) {
  //   case ARIA_KEY_W:
  //     pos.y -= camera_speed;
  //     break;
  //   case ARIA_KEY_A:
  //     pos.x += camera_speed;
  //     break;
  //   case ARIA_KEY_S:
  //     pos.y += camera_speed;
  //     break;
  //   case ARIA_KEY_D:
  //     pos.x -= camera_speed;
  //     break;
  //   case ARIA_KEY_R:
  //     pos = {0.0f, 0.0f, 0.0f};
  // }
  // mOrthoCamera.set_position(pos);
  // return false;
  // }

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

  const float camera_move_speed = 5.0f;
  const float camera_rotate_speed = 90.0f;

  const float square_move_speed = 1.0f;

  glm::vec3 mSquarePosition;
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
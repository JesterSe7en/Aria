#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("Example Layer"), mSquarePosition(0.0f) {
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
  mTriangleShader.reset(ARIA::Shader::Create("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicTriangle.shader"));

  // --------------- Rendering SQUARE ---------------
  mSquareVA.reset(ARIA::VertexArray::create());

  float squareVertices[3 * 4] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};

  mSquareVB.reset(ARIA::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

  mSquareVB->set_layout({{ARIA::ShaderPrimitiveType::Float3, "a_Position"}});
  mSquareVA->add_vertex_buffer(mSquareVB);

  uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

  mSquareIB.reset(ARIA::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
  mSquareVA->set_index_buffer(mSquareIB);

  mFlatColorShader.reset(ARIA::Shader::Create("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/flatColor.shader"));

  mOrthoCamera.set_position({0.0f, 0.0f, 0.0f});
}

void ExampleLayer::on_update(ARIA::Timestep delta_time) {
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

  // renders tiles (not ideally how to do this, just example)
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm ::vec3(0.1f));

  glm::vec4 red_color(0.906, 0.38f, 0.38f, 1.0f);
  glm::vec4 blue_color(0.427f, 0.663f, 0.894f, 1.0f);

  std::dynamic_pointer_cast<ARIA::OpenGLShader>(mFlatColorShader)
      ->set_uniform_4f("u_Color", mSquareColor.r, mSquareColor.g, mSquareColor.b, mSquareColor.a);

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
      if (x % 2) {
        std::dynamic_pointer_cast<ARIA::OpenGLShader>(mFlatColorShader)
            ->set_uniform_4f("u_Color", red_color.r, red_color.g, red_color.b, red_color.a);
      } else {
        std::dynamic_pointer_cast<ARIA::OpenGLShader>(mFlatColorShader)
            ->set_uniform_4f("u_Color", blue_color.r, blue_color.g, blue_color.b, blue_color.a);
      }
      ARIA::Renderer::submit(mFlatColorShader, mSquareVA, transform);
    }
  }

  ARIA::Renderer::submit(mTriangleShader, mTriangleVA);

  ARIA::Renderer::end_scene();
}

void ExampleLayer::on_event(ARIA::Event& event) {
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

void ExampleLayer::on_imgui_render() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
  ImGui::End();
}
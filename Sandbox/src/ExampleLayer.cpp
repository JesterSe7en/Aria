#include "ExampleLayer.h"
#include "Platform/OpenGL/OpenGlShader.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer() : Layer("Example Layer"), mSquarePosition(0.0f) {
  // // --------------- Rendering TRIANGLE ---------------
  // mTriangleVA.reset(ARIA::VertexArray::create());

  // // vertex buffer
  // float triangleVertices[3 * 7] = {
  //     -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
  //     0.3f,  0.8f,  1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f,
  // };
  // mTriangleVB.reset(ARIA::VertexBuffer::create(triangleVertices, sizeof(triangleVertices)));
  // ARIA::BufferLayout layout = {{ARIA::ShaderPrimitiveType::Float3, "a_Position"},
  //                              {ARIA::ShaderPrimitiveType::Float4, "a_Color"}};
  // mTriangleVB->set_layout(layout);
  // mTriangleVA->add_vertex_buffer(mTriangleVB);

  // // index buffer
  // uint32_t triangleIndicies[3] = {0, 1, 2};
  // mTriangleIB.reset(ARIA::IndexBuffer::create(triangleIndicies, sizeof(triangleIndicies) / sizeof(uint32_t)));
  // mTriangleVA->set_index_buffer(mTriangleIB);

  // // Why doesn't this accept relative path?
  // mTriangleShader.reset(ARIA::Shader::Create("C:/Users/alyxc/Workspace/Aria/Aria/res/shaders/basicTriangle.shader"));

  // --------------- Rendering SQUARE ---------------
  mSquareVA = aria::VertexArray::Create();

  float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                 0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

  mSquareVB = aria::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

  mSquareVB->SetLayout(
      {{aria::ShaderPrimitiveType::Float3, "a_Position"}, {aria::ShaderPrimitiveType::Float2, "a_TexCoord"}});
  mSquareVA->AddVertexBuffer(mSquareVB);

  uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

  mSquareIB = aria::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
  mSquareVA->SetIndexBuffer(mSquareIB);

  auto flatColorShader = mShaderLibrary.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/flatColor.glsl");
  auto textureShader = mShaderLibrary.Load("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/shaders/texture.glsl");

  mTexture2D =
      aria::Texture2D::Create("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/textures/missing_texture_checkboard.png");
  mCatTexture2D = aria::Texture2D::Create("C:/Users/alyxc/Workspace/Aria/Sandbox/assets/textures/cat_transparent.png");

  std::dynamic_pointer_cast<aria::OpenGlShader>(textureShader)->Bind();
  // this indicates what texture slot to load in
  std::dynamic_pointer_cast<aria::OpenGlShader>(textureShader)->SetUniform1I("u_Texture", 0);

  mOrthoCamera.SetPosition({0.0f, 0.0f, 0.0f});
}

void ExampleLayer::OnAttach() {}

void ExampleLayer::OnDetach() {}

void ExampleLayer::OnUpdate(aria::Timestep delta_time) {
#pragma region Camera Control
  auto position = mOrthoCamera.GetPosition();
  auto rotation = mOrthoCamera.GetRotation();

  if (aria::Input::IsKeyPressed(ARIA_KEY_W)) {
    position.y -= camera_move_speed * delta_time;
  } else if (aria::Input::IsKeyPressed(ARIA_KEY_S)) {
    position.y += camera_move_speed * delta_time;
  }

  if (aria::Input::IsKeyPressed(ARIA_KEY_A)) {
    position.x += camera_move_speed * delta_time;
  } else if (aria::Input::IsKeyPressed(ARIA_KEY_D)) {
    position.x -= camera_move_speed * delta_time;
  }

  if (aria::Input::IsKeyPressed(ARIA_KEY_R)) {
    mOrthoCamera.SetPosition({0.0f, 0.0f, 0.0f});
    mOrthoCamera.SetRotation(0.0f);
    return;
  }

  // ---------------------------------------------

  if (aria::Input::IsKeyPressed(ARIA_KEY_J)) {
    mSquarePosition.x -= square_move_speed * delta_time;
  } else if (aria::Input::IsKeyPressed(ARIA_KEY_L)) {
    mSquarePosition.x += square_move_speed * delta_time;
  }

  if (aria::Input::IsKeyPressed(ARIA_KEY_I)) {
    mSquarePosition.y += square_move_speed * delta_time;
  } else if (aria::Input::IsKeyPressed(ARIA_KEY_K)) {
    mSquarePosition.y -= square_move_speed * delta_time;
  }

  // ----------------------------------------------

  if (aria::Input::IsKeyPressed(ARIA_KEY_LEFT)) { rotation += camera_rotate_speed * delta_time; }

  if (aria::Input::IsKeyPressed(ARIA_KEY_RIGHT)) { rotation -= camera_rotate_speed * delta_time; }

  mOrthoCamera.SetPosition(position);
  mOrthoCamera.SetRotation(rotation);

#pragma endregion

  // delta_time with the float operator is returning time in SECONDS
  aria::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
  aria::RenderCommand::Clear();

  aria::Renderer::BeginScene(mOrthoCamera);

  // renders tiles (not ideally how to do this, just example)
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

      std::dynamic_pointer_cast<aria::OpenGlShader>(mShaderLibrary.Get("flatColor"))
          ->SetUniform4F("u_Color", mSquareColor.r, mSquareColor.g, mSquareColor.b, mSquareColor.a);

      aria::Renderer::Submit(mShaderLibrary.Get("flatColor"), mSquareVA, transform);
    }
  }

  // Render a square 1.5x time the size
  // ARIA::Renderer::submit(mFlatColorShader, mSquareVA, glm::scale(glm::mat4(1.0f), glm ::vec3(1.5f)));
  mTexture2D->Bind();
  aria::Renderer::Submit(mShaderLibrary.Get("texture"), mSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

  mCatTexture2D->Bind();
  aria::Renderer::Submit(mShaderLibrary.Get("texture"), mSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

  // ARIA::Renderer::submit(mTriangleShader, mTriangleVA);

  aria::Renderer::EndScene();
}

void ExampleLayer::OnEvent(aria::Event &event) {
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

void ExampleLayer::OnImGuiRender() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
  ImGui::End();
}

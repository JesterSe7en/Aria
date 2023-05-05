#pragma once
#include <Aria.h>

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer();

  void on_update(ARIA::Timestep delta_time) override;
  void on_event(ARIA::Event& event) override;

  void on_imgui_render() override;

 private:
  ARIA::OrthographicCamera mOrthoCamera = ARIA::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
  ARIA::Ref<ARIA::Shader> mTriangleShader;
  ARIA::Ref<ARIA::VertexBuffer> mTriangleVB;
  ARIA::Ref<ARIA::IndexBuffer> mTriangleIB;
  ARIA::Ref<ARIA::VertexArray> mTriangleVA;

  ARIA::Ref<ARIA::VertexArray> mSquareVA;
  ARIA::Ref<ARIA::VertexBuffer> mSquareVB;
  ARIA::Ref<ARIA::IndexBuffer> mSquareIB;
  ARIA::Ref<ARIA::Shader> mFlatColorShader;

  const float camera_move_speed = 5.0f;
  const float camera_rotate_speed = 90.0f;
  const float square_move_speed = 1.0f;

  glm::vec3 mSquarePosition;
  glm::vec4 mSquareColor = {0.427f, 0.663f, 0.894f, 1.0f};
};

#pragma once

#include <Aria.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public ARIA::Layer {
 public:
  ExampleLayer();

  void on_update(ARIA::Timestep delta_time) override;
  void on_event(ARIA::Event& event) override ;

  void on_imgui_render() override;

 private:
  ARIA::OrthographicCamera mOrthoCamera = ARIA::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
  std::shared_ptr<ARIA::Shader> mTriangleShader;
  std::shared_ptr<ARIA::VertexBuffer> mTriangleVB;
  std::shared_ptr<ARIA::IndexBuffer> mTriangleIB;
  std::shared_ptr<ARIA::VertexArray> mTriangleVA;

  std::shared_ptr<ARIA::VertexArray> mSquareVA;
  std::shared_ptr<ARIA::VertexBuffer> mSquareVB;
  std::shared_ptr<ARIA::IndexBuffer> mSquareIB;
  std::shared_ptr<ARIA::Shader> mFlatColorShader;

  const float camera_move_speed = 5.0f;
  const float camera_rotate_speed = 90.0f;

  const float square_move_speed = 1.0f;

  glm::vec3 mSquarePosition;
  glm::vec4 mSquareColor = {0.427f, 0.663f, 0.894f, 1.0f};
};
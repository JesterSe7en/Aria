#pragma once

#include "Aria/Renderer/Shader.h"
#include <Aria.h>

class ExampleLayer : public aria::Layer {
 public:
  ExampleLayer();

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(aria::Timestep delta_time) override;
  void OnEvent(aria::Event &event) override;

  void OnImGuiRender() override;

 private:
  aria::OrthographicCamera mOrthoCamera = aria::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
  aria::Ref<aria::Shader> mTriangleShader;
  aria::Ref<aria::VertexBuffer> mTriangleVB;
  aria::Ref<aria::IndexBuffer> mTriangleIB;
  aria::Ref<aria::VertexArray> mTriangleVA;

  aria::Ref<aria::VertexArray> mSquareVA;
  aria::Ref<aria::VertexBuffer> mSquareVB;
  aria::Ref<aria::IndexBuffer> mSquareIB;
  aria::Ref<aria::Shader> mFlatColorShader;

  aria::Ref<aria::Shader> mTextureShader;

  aria::Ref<aria::Texture2D> mTexture2D, mCatTexture2D;

  aria::ShaderLibrary mShaderLibrary;

  const float camera_move_speed = 5.0f;
  const float camera_rotate_speed = 90.0f;
  const float square_move_speed = 1.0f;

  glm::vec3 mSquarePosition;
  glm::vec4 mSquareColor = {0.427f, 0.663f, 0.894f, 1.0f};
};

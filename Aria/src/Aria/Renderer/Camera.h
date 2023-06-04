#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace aria {

class OrthographicCamera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top);
  ~OrthographicCamera() = default;

  void SetPosition(const glm::vec3 &position);
  void SetRotation(float rotation);

  const glm::vec3 &GetPosition() const { return position_; }
  float GetRotation() const { return rotation_; }

  const glm::mat4 &GetViewMatrix() const { return view_matrix_; }
  const glm::mat4 &GetProjectionMatrix() const { return projection_matrix_; }
  const glm::mat4 &GetVpMatrix() const { return vp_matrix_; }

 private:
  void RecalculateViewMatrix();

 private:
  glm::mat4 view_matrix_ = glm::mat4(1.0f);
  glm::mat4 projection_matrix_;
  glm::mat4 vp_matrix_;// for caching purposes - view projection matrix

  glm::vec3 position_ = {0.0f, 0.0f, 0.0f};

  float rotation_ = 0.0f;
};

class PerspectiveCamera {
 public:
  PerspectiveCamera(float top, float right, float bottom, float left);
  ~PerspectiveCamera() = default;

 private:
  glm::mat4 view_matrix_;
  glm::mat4 projection_matrix_;
  glm::mat4 vp_matrix_;// for caching purposes

  glm::vec3 position_;

  // need a quaternion here to track rotation
};
}// namespace aria

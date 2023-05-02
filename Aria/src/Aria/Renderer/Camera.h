#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>

namespace ARIA {

class OrthographicCamera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top);
  ~OrthographicCamera() = default;

  void set_position(const glm::vec3 &position);
  void set_rotation(float rotation);

  const glm::vec3 &get_position() const { return mPosition; }
  float get_rotation() const { return mRotation; }

  const glm::mat4 &get_view_matrix() const { return mViewMatrix; }
  const glm::mat4 &get_projection_matrix() const { return mProjectionMatrix; }
  const glm::mat4 &get_vp_matrix() const { return mVPMatrix; }

 private:
  void recalculate_view_matrix();

 private:
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectionMatrix;
  glm::mat4 mVPMatrix;  // for caching purposes - view projection matrix

  glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};

  float mRotation = 0.0f;
};

class PerspectiveCamera {
 public:
  PerspectiveCamera(float top, float right, float bottom, float left);
  ~PerspectiveCamera() = default;

 private:
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectionMatrix;
  glm::mat4 mVPMatrix;  // for caching purposes

  glm::vec3 mPosition;

  // need a quaternion here to track rotation
};
}  // namespace ARIA

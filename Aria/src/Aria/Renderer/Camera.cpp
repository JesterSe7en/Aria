#include "ariapch.h"
#include "Camera.h"

#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aria {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : projection_matrix_(glm::ortho(left, right, bottom, top)) {
  vp_matrix_ = projection_matrix_ * view_matrix_;
}

void OrthographicCamera::SetPosition(const glm::vec3 &position) {
  position_ = position;
  RecalculateViewMatrix();
}
void OrthographicCamera::SetRotation(float rotation) {
  rotation_ = rotation;
  RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3(0, 0, 1));

  view_matrix_ = glm::inverse(transform);

  // this order matters as OpenGL is column major
  vp_matrix_ = projection_matrix_ * view_matrix_;
}
}  // namespace ARIA

#include "ariapch.h"

#include "Camera.h"

#include <glm/matrix.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ARIA {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : mProjectionMatrix(glm::ortho(left, right, bottom, top)) {
  mVPMatrix = mProjectionMatrix * mViewMatrix;
}

void OrthographicCamera::set_position(const glm::vec3 &position) {
  mPosition = position;
  recalculate_view_matrix();
}
void OrthographicCamera::set_rotation(float rotation) {
  mRotation = rotation;
  recalculate_view_matrix();
}

void OrthographicCamera::recalculate_view_matrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));

  mViewMatrix = glm::inverse(transform);

  // this order matters as OpenGL is column major
  mVPMatrix = mProjectionMatrix * mViewMatrix;
}
}  // namespace ARIA

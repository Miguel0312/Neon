#include "scene/camera.h"
#include <cmath>

namespace Neon {
Ray Camera::getRay(unsigned int i, unsigned int j) {
  if (!m_computed) {
    prepareForRender();
  }

  return Ray(m_center, m_topLeftVector + i * m_deltaV + j * m_deltaU);
}

void Camera::prepareForRender() {
  m_computed = true;
  float viewportHeight = 2 * std::tan(m_verticalFOV / 2) * m_focalLength;
  float viewportWidth = viewportHeight * m_width / m_height;

  Vector3f right = m_forward.cross(m_up);
  m_deltaU = (viewportWidth / m_width) * right;
  m_deltaV = -(viewportHeight / m_height) * m_up;

  m_topLeftVector = m_up * viewportHeight / 2. - right * viewportWidth / 2. +
                    0.5 * m_deltaU + 0.5 * m_deltaV + m_forward * m_focalLength;
}
} // namespace Neon

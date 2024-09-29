#include "scene/camera.h"
#include "math/vector.h"
#include "utils/sampling/warp.h"
#include "utils/utils.h"

namespace Neon {
Ray Camera::getRay(unsigned int i, unsigned int j, Sampler *sampler) {
  if (!m_computed) {
    prepareForRender();
  }

  Vector3f pixelCenter = m_topLeftVector + i * m_deltaV + j * m_deltaU;
  // Transform the sample from the interval [0,1]^2 to [-0.5,0.5]^2
  Point2f sampleNormalized = sampler->next2D() - Point2f(0.5f, 0.5f);

  Vector3f dir = pixelCenter + sampleNormalized.x() * m_deltaV +
                 sampleNormalized.y() * m_deltaU;

  Ray ray(m_center, dir.normalize());

  if (m_lensRadius > 0) {
    Point2f delta = m_lensRadius * Warp::squareToUniformDisk(sampler->next2D());

    float t = m_focalLength / ray.dir.dot(m_forward);

    Point3f p = ray(t);
    ray.origin = m_center + delta.x() * m_lensU + delta.y() * m_lensV;
    ray.dir = (p - ray.origin).normalize();
  }

  return ray;
}

void Camera::prepareForRender() {
  m_computed = true;
  float viewportHeight =
      2 * std::tan(toRadians(m_verticalFOV / 2)) * m_focalLength;
  float viewportWidth = viewportHeight * m_width / m_height;

  Vector3f right = m_forward.cross(m_up);
  m_deltaU = (viewportWidth / m_width) * right;
  m_deltaV = -(viewportHeight / m_height) * m_up;

  m_lensU = m_deltaU.normalized();
  m_lensV = m_deltaV.normalized();

  m_topLeftVector = m_up * viewportHeight / 2. - right * viewportWidth / 2. +
                    0.5 * m_deltaU + 0.5 * m_deltaV + m_forward * m_focalLength;
}
} // namespace Neon

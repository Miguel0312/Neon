#ifndef NEON_CAMERA_H
#define NEON_CAMERA_H

#include "math/point.h"
#include "math/ray.h"
#include "math/vector.h"

namespace Neon {
class Camera {
public:
  Camera() = default;

  // Returns a ray starting from the camera center and passing by the (i,j)
  // pixel in the viewport
  Ray getRay(unsigned int i, unsigned int j);

  inline void setWidth(unsigned int width) { m_width = width; }

  inline void setHeight(unsigned int height) { m_height = height; }

  inline void setVerticalFOV(float fov) { m_verticalFOV = fov; }

  inline void setCenter(const Point3f &center) { m_center = center; }

  inline void setForward(const Vector3f &forward) { m_forward = forward; }

  inline void setUp(const Vector3f &up) { m_up = up; }

  inline void setFocalLength(float focalLength) { m_focalLength = focalLength; }

  unsigned int getWidth() const { return m_width; }

  unsigned int getHeight() const { return m_height; }

  float getVerticalFOV() const { return m_verticalFOV; }

  Point3f getCenter() const { return m_center; }

  Vector3f getForward() const { return m_forward; }

  Vector3f getUp() const { return m_up; }

  float getFocalLength() const { return m_focalLength; }

private:
  unsigned int m_width = 800, m_height = 600;
  float m_verticalFOV = 20;
  Point3f m_center{0};
  // Normalized vectors in world coordinates
  Vector3f m_forward{0, 0, -1}, m_up{0, 1, 0};
  float m_focalLength = 1;

  bool m_computed = false;
  // Vector from m_center to the pixel (0,0)
  Vector3f m_topLeftVector;
  Vector3f m_deltaU, m_deltaV;

  void prepareForRender();
};
} // namespace Neon

#endif

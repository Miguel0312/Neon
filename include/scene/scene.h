#ifndef NEON_SCENE_H
#define NEON_SCENE_H

#include "scene/camera.h"
#include "scene/shape.h"
#include <memory>
#include <vector>
namespace Neon {
class Scene {
public:
  Scene() = default;

  ~Scene() = default;

  void addShape(std::unique_ptr<Shape> shape);

  void setCamera(std::unique_ptr<Camera> &camera);

  void setFilename(const std::string &filename);

  void render();

private:
  std::vector<std::unique_ptr<Shape>> m_shapes;
  std::unique_ptr<Camera> m_camera;
  std::string m_filename = "images/test.png";
};
} // namespace Neon

#endif

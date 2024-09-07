#include "utils/color.h"
#include "utils/image.h"
#include <iostream>
#include <memory>
#include <scene/scene.h>

namespace Neon {
void Scene::addShape(std::unique_ptr<Shape> shape) {
  m_shapes.push_back(std::move(shape));
}

void Scene::setCamera(std::unique_ptr<Camera> &camera) {
  m_camera = std::move(camera);
}

void Scene::setFilename(const std::string &filename) { m_filename = filename; }

void Scene::render() {
  if (!m_camera) {
    std::cout << "ERROR: Camera was not set" << std::endl;
    return;
  }
  int width = m_camera->getWidth(), height = m_camera->getHeight();

  std::vector<std::vector<Neon::Color>> pixels(height,
                                               std::vector<Neon::Color>(width));

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Ray ray = m_camera->getRay(i, j);
      float a = 0.5 * (1.0 + ray.dir[1]);
      pixels[i][j] =
          (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
  }

  Neon::createImage(m_filename, pixels, width, height);

  std::cout << "Finished rendering " << m_filename << std::endl;
}
} // namespace Neon

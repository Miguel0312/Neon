#include "scene/integrators/integrator.h"
#include "scene/shape.h"
#include "utils/color.h"
#include "utils/image.h"
#include <iostream>
#include <memory>
#include <scene/scene.h>

namespace Neon {
void Scene::addShape(std::unique_ptr<Shape> &shape) {
  m_shapes.push_back(std::move(shape));
}

void Scene::setCamera(std::unique_ptr<Camera> &camera) {
  m_camera = std::move(camera);
}
void Scene::setIntegrator(std::unique_ptr<Integrator> &integrator) {
  m_integrator = std::move(integrator);
}

void Scene::setFilename(const std::string &filename) { m_filename = filename; }

bool Scene::rayIntersection(const Ray &r, ShapeIntersectionRecord &rec) {
  bool result = false;
  Intervalf tInterval(0.1, 100);
  for (auto &shape : m_shapes) {
    if (shape->intersect(r, tInterval, rec)) {
      tInterval.setMax(rec.t);
      result = true;
    }
  }

  return result;
}

void Scene::render() {
  if (!m_camera) {
    std::cout << "ERROR: Camera was not set" << std::endl;
    return;
  }
  if (!m_integrator) {
    std::cout << "ERROR: Integrator was not set" << std::endl;
    return;
  }
  int width = m_camera->getWidth(), height = m_camera->getHeight();

  std::vector<std::vector<Neon::Color>> pixels(height,
                                               std::vector<Neon::Color>(width));

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      Ray ray = m_camera->getRay(i, j);
      pixels[i][j] = m_integrator->Li(this, ray);
    }
  }

  Neon::createImage(m_filename, pixels, width, height);

  std::cout << "Finished rendering " << m_filename << std::endl;
}
} // namespace Neon

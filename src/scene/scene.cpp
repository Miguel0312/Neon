#include "scene/integrators/integrator.h"
#include "scene/shape.h"
#include "utils/color.h"
#include "utils/image.h"
#include "utils/threadPool.h"
#include <iostream>
#include <memory>
#include <scene/scene.h>

namespace Neon {
void Scene::addShape(std::unique_ptr<Shape> &shape) {
  m_shapes.push_back(std::move(shape));
}

void Scene::setCamera(std::unique_ptr<Camera> &camera) {
  m_camera = std::move(camera);
  m_pixels = std::vector<std::vector<Neon::Color>>(
      m_camera->getHeight(), std::vector<Neon::Color>(m_camera->getWidth()));
}

void Scene::setIntegrator(std::unique_ptr<Integrator> &integrator) {
  m_integrator = std::move(integrator);
}

void Scene::setSampler(std::unique_ptr<Sampler> &sampler) {
  m_sampler = std::move(sampler);
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
  if (!m_sampler) {
    std::cout << "ERROR: Sampler was not set" << std::endl;
    return;
  }
  int width = m_camera->getWidth(), height = m_camera->getHeight();

  ThreadPool threadPool;

  const int step = 64;

  for (int i = 0; i < height; i += step) {
    for (int j = 0; j < width; j += step) {

      threadPool.queueJob([i, j, width, height, this] {
        // The sampler needs to be cloned to insure reproducibility when using a
        // multi threaded rendering process
        std::unique_ptr<Sampler> sampler = m_sampler->clone();
        int r = std::min(i + step, height), b = std::min(j + step, width);
        for (int k = i; k < r; k++) {
          for (int l = j; l < b; l++) {
            Ray ray = m_camera->getRay(k, l);
            m_pixels[k][l] = m_integrator->Li(this, ray, sampler.get());
          }
        }
      });
    }
  }

  while (threadPool.busy())
    ;

  threadPool.stop();

  Neon::createImage(m_filename, m_pixels, width, height);

  std::cout << "Finished rendering " << m_filename << std::endl;
}
} // namespace Neon

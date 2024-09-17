#include "scene/accelerators/accelerator.h"
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
  m_box = m_box.merge(shape->getBoundingBox());
  m_shapes.push_back(std::move(shape));
}

void Scene::addLight(std::unique_ptr<Light> &light) {
  m_lights.push_back(std::move(light));
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

void Scene::setAccelerator(std::unique_ptr<Accelerator> &accelerator) {
  m_accelerator = std::move(accelerator);
  m_accelerator->setScene(this);
}

void Scene::setFilename(const std::string &filename) { m_filename = filename; }

bool Scene::rayIntersection(const Ray &r, ShapeIntersectionRecord &rec) {
  return m_accelerator->rayIntersection(r, rec);
}

Color Scene::sampleLight(LightSampleRecord &rec, Sampler *sampler) const {
  int index = m_lightDistribution.sample(sampler->next1D());
  return m_lights[index]->sample(this, rec, sampler);
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
  if (!m_accelerator) {
    std::cout << "ERROR: Accelerator was not set" << std::endl;
    return;
  }
  m_accelerator->preprocess();
  int width = m_camera->getWidth(), height = m_camera->getHeight();

  for (auto &light : m_lights) {
    float pdf = light->getPDF();
    m_lightDistribution.add(pdf);
    m_lightPDFSum += pdf;
  }

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
            Color result(0);
            for (int m = 0; m < m_sampleCount; m++) {
              Ray ray = m_camera->getRay(k, l, sampler->next2D());
              result += m_integrator->Li(this, ray, sampler.get());
            }
            m_pixels[k][l] = result / m_sampleCount;
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

#include "math/point.h"
#include "scene/camera.h"
#include "scene/integrators/normalIntegrator.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "scene/sphere.h"
#include "utils/sampling/independentSampler.h"
#include "utils/visualizer.h"
#include <math/vector.h>
#include <memory>
#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 800
#define HEIGHT 600

using namespace Neon;

int main() {
  std::vector<std::vector<Neon::Color>> pixels(HEIGHT,
                                               std::vector<Neon::Color>(WIDTH));

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setWidth(WIDTH), camera->setHeight(HEIGHT);
  camera->setCenter({0, 0, 2});

  std::unique_ptr<Integrator> normalIntegrator =
      std::make_unique<NormalIntegrator>();

  std::unique_ptr<Sampler> sampler = std::make_unique<IndependentSampler>();

  std::unique_ptr<Shape> sphere1 = std::make_unique<Sphere>(0, 1);
  std::unique_ptr<Shape> sphere2 =
      std::make_unique<Sphere>(Point3f(-5, -5, -3), 1);
  std::unique_ptr<Shape> ground =
      std::make_unique<Sphere>(Point3f(0, -102, 0), 100);

  Scene scene;
  scene.addShape(sphere2);
  for (int i = 0; i < 100; i++) {
    std::unique_ptr<Shape> sphere =
        std::make_unique<Sphere>(Point3f(-5 + i % 10, -5 + i / 10, 0), 0.5);
    scene.addShape(sphere);
  }
  scene.setCamera(camera);
  scene.setIntegrator(normalIntegrator);
  scene.setSampler(sampler);

  Visualizer visualizer(&scene);

  visualizer.render();

  // scene.render();

  return 0;
}

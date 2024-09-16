#include "math/point.h"
#include "reflection/lambertian.h"
#include "scene/accelerators/accelerator.h"
#include "scene/accelerators/octree.h"
#include "scene/camera.h"
#include "scene/integrators/whittedIntegrator.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "scene/sphere.h"
#include "scene/triangle.h"
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

  std::unique_ptr<Integrator> integrator =
      std::make_unique<WhittedIntegrator>(10);

  std::unique_ptr<Sampler> sampler = std::make_unique<IndependentSampler>();
  Lambertian basicMaterial(Color(0.5, 0.5, 0.5));

  std::unique_ptr<Accelerator> accelerator = std::make_unique<Octree>();

  std::unique_ptr<Shape> triangle1 = std::make_unique<Triangle>(
      Point3f(0, 0, 0), Point3f(1, 0, 0), Point3f(0, 1, 0), &basicMaterial);
  Scene scene;

  scene.addShape(triangle1);

  scene.setCamera(camera);
  scene.setIntegrator(integrator);
  scene.setSampler(sampler);
  scene.setAccelerator(accelerator);
  scene.setSampleCount(64);

  Visualizer visualizer(&scene);

  visualizer.render();

  return 0;
}

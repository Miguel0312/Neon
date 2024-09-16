#include "math/point.h"
#include "reflection/lambertian.h"
#include "reflection/metallic.h"
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
  std::unique_ptr<Accelerator> accelerator = std::make_unique<Octree>();

  Lambertian groundMaterial = Lambertian(Color(0.8, 0.8, 0.0));
  Lambertian centerMaterial = Lambertian(Color(0.1, 0.2, 0.5));
  Metallic leftMaterial = Metallic(Color(0.8, 0.8, 0.8));
  Metallic rightMaterial = Metallic(Color(0.8, 0.6, 0.2));

  std::unique_ptr<Shape> sphere1 =
      std::make_unique<Sphere>(Point3f(0, 0, -0.2), 0.5f, &centerMaterial);
  std::unique_ptr<Shape> sphere2 =
      std::make_unique<Sphere>(Point3f(-1.0, 0.0, 0.0), 0.5f, &leftMaterial);
  std::unique_ptr<Shape> sphere3 =
      std::make_unique<Sphere>(Point3f(1, 0, 0.0), 0.5f, &rightMaterial);
  std::unique_ptr<Shape> ground = std::make_unique<Sphere>(
      Point3f(0, -100.5, 0.0), 100.0f, &groundMaterial);

  Scene scene;

  scene.addShape(sphere1);
  scene.addShape(sphere2);
  scene.addShape(sphere3);
  scene.addShape(ground);

  scene.setCamera(camera);
  scene.setIntegrator(integrator);
  scene.setSampler(sampler);
  scene.setAccelerator(accelerator);
  scene.setSampleCount(1024);

  Visualizer visualizer(&scene);

  visualizer.render();

  return 0;
}

#include "math/point.h"
#include "scene/camera.h"
#include "scene/integrators/normalIntegrator.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "scene/sphere.h"
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

  std::unique_ptr<Shape> sphere = std::make_unique<Sphere>(0, 1);

  Scene scene;
  scene.addShape(sphere);
  scene.setCamera(camera);
  scene.setIntegrator(normalIntegrator);

  Visualizer visualizer(&scene);

  visualizer.render();

  // scene.render();

  return 0;
}

#include "math/point.h"
#include "scene/camera.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "scene/sphere.h"
#include <iostream>
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

  std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(0, 1);

  Scene scene;
  scene.addShape(std::move(sphere));
  scene.setCamera(camera);

  scene.render();

  return 0;
}

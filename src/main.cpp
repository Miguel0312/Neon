#include "math/point.h"
#include "scene/camera.h"
#include "scene/shape.h"
#include "scene/sphere.h"
#include <iostream>
#include <math/vector.h>
#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 800
#define HEIGHT 600

using namespace Neon;

int main() {
  std::vector<std::vector<Neon::Color>> pixels(HEIGHT,
                                               std::vector<Neon::Color>(WIDTH));

  Neon::Camera camera;
  camera.setWidth(WIDTH), camera.setHeight(HEIGHT);
  camera.setCenter({0, 0, 2});

  ShapeIntersectionRecord rec;
  Sphere sphere(0, 1);

  if (sphere.intersect(camera.getRay(HEIGHT / 2, WIDTH / 2), 0, 100, rec)) {
    std::cout << rec.p << std::endl;
  }

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      pixels[i][j].r = double(i) / (HEIGHT - 1);
      pixels[i][j].g = double(j) / (WIDTH - 1);
      pixels[i][j].b = 0;
    }
  }

  Neon::createImage("images/test.png", pixels, WIDTH, HEIGHT);

  return 0;
}

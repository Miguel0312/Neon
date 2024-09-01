#include "math/point.h"
#include "math/ray.h"
#include "math/vector.h"
#include <iostream>
#include <math/vector.h>
#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 256
#define HEIGHT 256

using namespace Neon;

int main() {
  std::vector<std::vector<Neon::Color>> pixels(HEIGHT,
                                               std::vector<Neon::Color>(WIDTH));

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      pixels[i][j].r = double(i) / (HEIGHT - 1);
      pixels[i][j].g = double(j) / (WIDTH - 1);
      pixels[i][j].b = 0;
    }
  }

  Neon::createImage("images/test.png", pixels, WIDTH, HEIGHT);

  Point3f p(0, 1, 0);
  Vector3f v(1, 0, 0);

  Ray r(p, v);

  std::cout << r(0.5) << std::endl;
  std::cout << r(1.0) << std::endl;

  return 0;
}

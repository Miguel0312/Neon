#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 256
#define HEIGHT 256

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

  return 0;
}

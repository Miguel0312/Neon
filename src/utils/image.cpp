#include <filesystem>
#include <thirdparty/stb/stb_image_write.h>
#include <utils/image.h>

namespace Neon {
void createImage(const std::string &filename,
                 const std::vector<std::vector<Color>> &pixels,
                 unsigned int width, unsigned int height) {
  std::filesystem::path path(filename);
  // Create the image folder if it does not exist
  if (path.has_parent_path()) {
    std::filesystem::create_directory(path.parent_path());
  }

  std::vector<unsigned char> data(3 * width * height);
  for (unsigned int i = 0; i < height; i++) {
    for (unsigned int j = 0; j < width; j++) {
      data[3 * (i * width + j) + 0] = int(255.999 * pixels[i][j].r);
      data[3 * (i * width + j) + 1] = int(255.999 * pixels[i][j].g);
      data[3 * (i * width + j) + 2] = int(255.999 * pixels[i][j].b);
    }
  }

  stbi_write_png("images/test.png", width, height, 3, data.data(), 3 * width);
}
} // namespace Neon

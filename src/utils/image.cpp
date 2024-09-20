#include <cmath>
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
  colorToBytes(pixels, data);

  stbi_write_png(filename.c_str(), width, height, 3, data.data(), 3 * width);
}

// Pass the bytes by reference to limit the amount of alocations
void colorToBytes(const std::vector<std::vector<Color>> &color,
                  std::vector<unsigned char> &bytes) {
  if (color.empty() || color[0].empty()) {
    return;
  }

  unsigned int width = color[0].size(), height = color.size();
  // Make sure we have enough room
  bytes.resize(3 * color.size() * color[0].size());
  for (unsigned int i = 0; i < height; i++) {
    for (unsigned int j = 0; j < width; j++) {
      // The square root operation allows us to do gamma correction
      // The most important is that it gives a better resolution to darker
      // colors
      bytes[3 * (i * width + j) + 0] = int(255.999 * std::sqrt(color[i][j].r));
      bytes[3 * (i * width + j) + 1] = int(255.999 * std::sqrt(color[i][j].g));
      bytes[3 * (i * width + j) + 2] = int(255.999 * std::sqrt(color[i][j].b));
    }
  }
}
} // namespace Neon

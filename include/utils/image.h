#ifndef NEON_IMAGE_H
#define NEON_IMAGE_H

#include "utils/color.h"
#include <string>
#include <vector>
namespace Neon {
// Creates a png image from a 2D vector of Color
void createImage(const std::string &filename,
                 const std::vector<std::vector<Color>> &pixels,
                 unsigned int width, unsigned int height);
} // namespace Neon

#endif

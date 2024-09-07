#ifndef NEON_COLOR_H
#define NEON_COLOR_H

namespace Neon {
struct Color {
  // RGB values of the color, should be between 0 and 1
  float r, g, b;

  Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

  Color operator*(float t) const { return Color(t * r, t * g, t * b); }

  Color operator+(const Color &other) const {
    return Color(r + other.r, g + other.g, b + other.b);
  }
};

inline Color operator*(float t, const Color &color) { return color * t; }

} // namespace Neon

#endif

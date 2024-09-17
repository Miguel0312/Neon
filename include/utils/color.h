#ifndef NEON_COLOR_H
#define NEON_COLOR_H

#include <ostream>
namespace Neon {
struct Color {
  // RGB values of the color, should be between 0 and 1
  float r, g, b;

  Color(float c = 0) : r(c), g(c), b(c) {}

  Color(float r, float g, float b) : r(r), g(g), b(b) {}

  Color operator*(float t) const { return Color(t * r, t * g, t * b); }

  Color operator/(float t) const { return (*this) * (1 / t); }

  Color operator+(const Color &other) const {
    return Color(r + other.r, g + other.g, b + other.b);
  }

  Color &operator+=(const Color &other) {
    r += other.r;
    g += other.g;
    b += other.b;

    return *this;
  }

  Color operator*(const Color &other) const {
    return Color(r * other.r, g * other.g, b * other.b);
  }
};

inline Color operator*(float t, const Color &color) { return color * t; }

inline std::ostream &operator<<(std::ostream &out, const Color &c) {
  out << '(' << c.r << ", " << c.g << ", " << c.b << ')';
  return out;
}

} // namespace Neon

#endif

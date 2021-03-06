#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdlib.h>

#include <utility>

namespace cslibs_math {
namespace color {
template <typename T>
struct Color {
  inline Color() = default;
  inline Color(const T _r, const T _g, const T _b) : r{_r}, g{_g}, b{_b} {}

  inline Color(const Color& other) = default;
  inline Color(Color&& other) = default;

  T r{0};
  T g{0};
  T b{0};
};
/**
 * @brief interpolateColor interpolates a color for a value v in [vmin, vmax]
 * @param v curremt value
 * @param vmin intervall minimum value (inclusive)
 * @param vmax invervall maximum value (inclusive)
 * @return the interpolated color (part of colormap)
 */
template <typename T>
inline Color<T> interpolateColor(T v, T vmin, T vmax) {
  Color<T> c{1.0, 1.0, 1.0};  // white
  T dv;

  if (v < vmin) v = vmin;
  if (v > vmax) v = vmax;
  dv = vmax - vmin;

  if (v < (vmin + 0.25 * dv)) {
    c.r = 0;
    c.g = static_cast<T>(4.0 * (v - vmin) / dv);
  } else if (v < (vmin + 0.5 * dv)) {
    c.r = 0;
    c.b = static_cast<T>(1.0 + 4.0 * (vmin + 0.25 * dv - v) / dv);
  } else if (v < (vmin + 0.75 * dv)) {
    c.r = static_cast<T>(4.0 * (v - vmin - 0.5 * dv) / dv);
    c.b = 0;
  } else {
    c.g = static_cast<T>(1.0 + 4.0 * (vmin + 0.75 * dv - v) / dv);
    c.b = 0;
  }

  return (c);
}

template <typename T>
inline Color<T> random() {
  return Color<T>{drand48(), drand48(), drand48()};
}

/**
 * @brief interpolateValue inverse of interpolateColor
 * @param c the color
 * @param vmin minmum value
 * @param vmax maximum value
 * @return the value v leading to interpolateColor(v, vmin, vmax)
 */
template <typename T>
inline T interpolateValue(const Color<T>& c, T vmin, T vmax) {
  T dv = vmax - vmin;
  T value = vmin;
  if (c.r == 0 && c.b < 1 && c.g < 1) {
    value = 0.25 * dv * c.g + vmin;
  } else if (c.r == 0 && c.g == 1 && c.b < 1) {
    value = 0.25 * dv * (2.0 - c.b) + vmin;
  } else if (c.r > 0 && c.r < 1.0 && c.g == 1.0 && c.b == 0) {
    value = 0.25 * dv * c.r + 0.5 * dv + vmin;
  } else if (c.r == 1.0) {
    value = 0.25 * dv * (4 - c.g) + vmin;
  }
  return value;
}

}  // namespace color
}  // namespace cslibs_math
#endif  // COLOR_HPP

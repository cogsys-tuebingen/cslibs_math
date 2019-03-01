#ifndef CSLIBS_MATH_2D_LINE_2D_HPP
#define CSLIBS_MATH_2D_LINE_2D_HPP

#include <cslibs_math_2d/linear/point.hpp>

namespace cslibs_math_2d {
template <typename T>
using Line2d = std::array<Point2d<T>, 2>;
}

#endif // CSLIBS_MATH_2D_LINE_2D_HPP

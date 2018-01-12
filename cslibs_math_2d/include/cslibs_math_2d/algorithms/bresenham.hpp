#ifndef CSLIBS_MATH_2D_BRESENHAM_HPP
#define CSLIBS_MATH_2D_BRESENHAM_HPP

#include <memory>
#include <array>

#include <cslibs_math_2d/linear/point.hpp>

namespace cslibs_math_2d {
namespace algorithms {
class Bresenham
{
public:
    using Ptr           = std::shared_ptr<Bresenham>;
    using index_t       = std::array<int, 2>;
    using point_t       = Point2d;

    inline Bresenham() :
        start_{{0,0}},
        end_{{0,0}}
    {
    }

    inline explicit Bresenham(const point_t &p0,
                              const point_t &p1,
                              const double resolution):
    Bresenham({{static_cast<int>(p0(0) / resolution), static_cast<int>(p0(1))}},
              {{static_cast<int>(p1(0) / resolution), static_cast<int>(p1(1))}})
    {
    }

    inline explicit Bresenham(const index_t     &start,
                              const index_t     &end) :
        start_(start),
        end_(end),
        steep_(std::abs(end[1] - start[1]) > std::abs(end[0] - start[0])),
        error_(0)
    {
        if(steep_) {
            std::swap(start_[0], start_[1]);
            std::swap(end_[0], end_[1]);
        }

        delta_[0]    = std::abs(end_[0] - start_[0]);
        delta_[1]    = std::abs(end_[1] - start_[1]);

        delta_error_ = delta_[1];
        index_       = start_;

        step_[0] = start_[0] < end_[0] ? 1 : -1;
        step_[1] = start_[1] < end_[1] ? 1 : -1;

    }

    inline virtual ~Bresenham()
    {
    }

    inline int x() const
    {
        return (steep_ ? index_[1] : index_[0]);
    }

    inline int y() const
    {
        return (steep_ ? index_[0] : index_[1]);
    }

    inline Bresenham& operator++()
    {
        return done() ? *this : iterate();
    }

    inline int distance2() const
    {
        auto sq = [](const int d) { return d*d;};
        return sq(index_[0] - end_[0]) + sq(index_[1] - end_[1]);
    }

    inline int traversed2() const
    {
        auto sq = [](const int d) { return d*d;};
        return sq(index_[0] - start_[0]) + sq(index_[1] - start_[1]);
    }

    inline bool done() const
    {
        return index_[0] == end_[0] && index_[1] == end_[1];
    }

private:
    inline Bresenham &iterate()
    {
        index_[0]       += step_[0];
        error_ += delta_error_;
        if(2 * error_ >= delta_[0]) {
            index_[1]       += step_[1];
            error_          -= delta_[0];
        }
        return *this;
    }

    index_t start_;
    index_t end_;
    index_t index_;
    index_t step_;
    index_t delta_;

    bool    steep_;
    int     error_;
    int     delta_error_;

};
}
}

#endif // CSLIBS_MATH_2D_BRESENHAM_HPP
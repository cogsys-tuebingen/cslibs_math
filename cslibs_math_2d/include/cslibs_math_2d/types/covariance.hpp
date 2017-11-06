#ifndef CSLIBS_MATH_2D_COVARIANCE_2D_HPP
#define CSLIBS_MATH_2D_COVARIANCE_2D_HPP

#include <array>
#include <vector>
#include <eigen3/Eigen/Core>

namespace cslibs_math_2d {
class Covariance2d {
public:
    const static std::size_t step_ = 3;
    const static std::size_t size_ = step_ * step_;
    using data_t = std::array<double, size_>;

    inline Covariance2d()
    {
        data_.fill(0.0);
    }

    inline Covariance2d(const data_t &data) :
        data_(data)
    {
    }

    inline double & operator () (const std::size_t i, const std::size_t j)
    {
        return data_[step_ * i + j];
    }

    inline double const & operator () (const std::size_t i, const std::size_t j) const
    {
        return data_[step_ * i + j];
    }

    inline void from(const std::vector<double> &data)
    {
        for(std::size_t i = 0 ; i < size_ ; ++i) {
            data_[i] = data[i];
        }
    }


    inline void from(const Eigen::Matrix3d &e)
    {
        for(std::size_t i = 0 ; i < step_ ; ++i) {
            for(std::size_t j = 0 ; j < step_ ; ++j) {
                 data_[step_ * i + j] = e(static_cast<long>(i), static_cast<long>(j));
            }
        }
    }

    inline Eigen::Matrix3d toEigen() const
    {
        Eigen::Matrix3d e;
        for(std::size_t i = 0 ; i < step_ ; ++i) {
            for(std::size_t j = 0 ; j < step_ ; ++j) {
                e(static_cast<long>(i),static_cast<long>(j)) = data_[step_ * i + j];
            }
        }
        return e;
    }

    inline void toEigen(Eigen::Matrix3d &e) const
    {
        for(std::size_t i = 0 ; i < step_ ; ++i) {
            for(std::size_t j = 0 ; j < step_ ; ++j) {
                e(static_cast<long>(i),static_cast<long>(j)) = data_[step_ * i + j];
            }
        }
    }
private:
    data_t data_;
}__attribute__ ((aligned (128)));
}

#endif // CSLIBS_MATH_2D_COVARIANCE_2D_HPP

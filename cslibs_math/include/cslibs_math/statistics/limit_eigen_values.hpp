#ifndef CSLIBS_MATH_LIMIT_COVARIANCE_HPP
#define CSLIBS_MATH_LIMIT_COVARIANCE_HPP

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>

namespace cslibs_math {
namespace statistics {

template<std::size_t exp>
struct LambdaRatio
{
    constexpr const static double value = 0.1 * LambdaRatio<exp - 1ul>::value;
};
template<>
struct LambdaRatio<0ul>
{
    constexpr const static double value = 1.0;
};

template<std::size_t Dim, std::size_t lambda_ratio_exponent>
/**
 * @brief The LimitCovariance struct is used to assure, that eigen values are not too small.
 *        This will probihit the distribution to collapse in one dimension, since eigen values
 *        represent the length of the eigen vectors, which build a local orthonormal system.
 */
struct LimitEigenValues
{
    const static constexpr double lambda_ratio = LambdaRatio<lambda_ratio_exponent>::value;

    using matrix_t        = Eigen::Matrix<double, Dim, Dim>;
    using eigen_values_t  = Eigen::Matrix<double, Dim, 1>;
    using eigen_vectors_t = Eigen::Matrix<double, Dim, Dim>;

    static inline void apply(matrix_t &matrix_io)
    {
        Eigen::EigenSolver<matrix_t> solver;
        solver.compute(matrix_io);
        const auto eigen_values  = solver.eigenvalues().real();
        const auto eigen_vectors = solver.eigenvectors().real();

        const double lambda = lambda_ratio * eigen_values.maxCoeff();
        matrix_t Lambda = matrix_t::Zero();
        for(std::size_t i = 0 ; i < Dim; ++i) {
            Lambda(i,i) = std::abs(eigen_values(i)) < std::abs(lambda) ? lambda : eigen_values(i);
        }
        matrix_io = eigen_vectors * Lambda * eigen_vectors.transpose();
    }
};

template<std::size_t Dim>
struct LimitEigenValues<Dim, 0ul>
{
    using matrix_t    = Eigen::Matrix<double, Dim, Dim>;
    using eigen_values_t  = Eigen::Matrix<double, Dim, 1>;
    using eigen_vectors_t = Eigen::Matrix<double, Dim, Dim>;

    static inline void apply(matrix_t &)
    {
    }
};

template<std::size_t Dim>
struct LimitEigenValuesByZero {
    using matrix_t        = Eigen::Matrix<double, Dim, Dim>;
    using eigen_values_t  = Eigen::Matrix<double, Dim, 1>;
    using eigen_vectors_t = Eigen::Matrix<double, Dim, Dim>;

    static inline void apply(matrix_t &matrix_io)
    {
        Eigen::EigenSolver<matrix_t> solver;
        solver.compute(matrix_io);
        const auto eigen_values  = solver.eigenvalues().real();
        const auto eigen_vectors = solver.eigenvectors().real();

        matrix_t Lambda = matrix_t::Zero();
        for(std::size_t i = 0 ; i < Dim; ++i) {
            Lambda(i,i) = std::max(eigen_values(i), 0.0);
        }
        matrix_io = eigen_vectors * Lambda * eigen_vectors.transpose();
    }
};
}
}


#endif // CSLIBS_MATH_LIMIT_COVARIANCE_HPP
#ifndef CSLIBS_MATH_SERIALIZATION_DISTRIBUTION_HPP
#define CSLIBS_MATH_SERIALIZATION_DISTRIBUTION_HPP

#include <yaml-cpp/yaml.h>

#include <cslibs_math/serialization/binary.hpp>
#include <cslibs_math/statistics/distribution.hpp>
#include <fstream>

namespace cslibs_math {
namespace serialization {
template <template <typename, std::size_t, std::size_t>
          class distribution_class_t,
          typename T, std::size_t Dim, std::size_t lambda_ratio_exponent>
struct binary;

template <typename T, std::size_t Dim, std::size_t lambda_ratio_exponent>
struct binary<cslibs_math::statistics::Distribution, T, Dim,
              lambda_ratio_exponent> {
  using distribution_t =
      cslibs_math::statistics::Distribution<T, Dim, lambda_ratio_exponent>;
  using sample_t = typename distribution_t::sample_t;
  using correlated_t = typename distribution_t::covariance_t;

  static const std::size_t size =
      sizeof(std::size_t) + Dim * sizeof(T) + Dim * Dim * sizeof(T);

  inline static std::size_t read(std::ifstream &in,
                                 distribution_t &distribution) {
    sample_t mean;
    correlated_t corr;

    std::size_t n = io<std::size_t>::read(in);

    if (n > 0) {  // data only exists if n > 0
      for (std::size_t i = 0; i < Dim; ++i) mean(i) = io<T>::read(in);

      for (std::size_t i = 0; i < Dim; ++i) {
        for (std::size_t j = 0; j < Dim; ++j) {
          corr(i, j) = io<T>::read(in);
        }
      }
      distribution = distribution_t(n, mean, corr);
    } else
      distribution = distribution_t();

    return size;
  }

  inline static void write(std::ofstream &out) {
    io<std::size_t>::write(0, out);
    /*
            for(std::size_t i = 0 ; i < Dim ; ++i)
                io<T>::write(0.0, out);

            for(std::size_t i = 0 ; i < Dim; ++i) {
                for(std::size_t j = 0 ; j < Dim ; ++j) {
                    io<T>::write(0.0, out);
                }
            }*/
  }

  inline static void write(const distribution_t &distribution,
                           std::ofstream &out) {
    const sample_t mean = distribution.getMean();
    const correlated_t corr = distribution.getCorrelated();
    const std::size_t n = distribution.getN();

    io<std::size_t>::write(n, out);

    for (std::size_t i = 0; i < Dim; ++i) io<T>::write(mean(i), out);

    for (std::size_t i = 0; i < Dim; ++i) {
      for (std::size_t j = 0; j < Dim; ++j) {
        io<T>::write(corr(i, j), out);
      }
    }
  }
};
}  // namespace serialization
}  // namespace cslibs_math

namespace YAML {
template <typename T, std::size_t Dim, std::size_t lambda_ratio_exponent>
struct convert<
    cslibs_math::statistics::Distribution<T, Dim, lambda_ratio_exponent>> {
  using sample_t = typename cslibs_math::statistics::Distribution<
      T, Dim, lambda_ratio_exponent>::sample_t;
  using covariance_t = typename cslibs_math::statistics::Distribution<
      T, Dim, lambda_ratio_exponent>::covariance_t;

  static Node encode(
      const cslibs_math::statistics::Distribution<T, Dim, lambda_ratio_exponent>
          &rhs) {
    Node n;
    n.push_back(rhs.getN());

    sample_t mean = rhs.getMean();
    for (std::size_t i = 0; i < Dim; ++i) n.push_back(mean(i));

    covariance_t correlated = rhs.getCorrelated();
    for (std::size_t i = 0; i < Dim; ++i)
      for (std::size_t j = 0; j < Dim; ++j) n.push_back(correlated(i, j));
    return n;
  }

  static bool decode(const Node &n, cslibs_math::statistics::Distribution<
                                        T, Dim, lambda_ratio_exponent> &rhs) {
    if (!n.IsSequence() || n.size() != (1 + Dim + Dim * Dim)) return false;

    std::size_t p = 0;
    std::size_t num = n[p++].as<std::size_t>();

    sample_t mean{sample_t::Zero()};
    for (std::size_t i = 0; i < Dim; ++i) {
      mean(i) = n[p++].as<T>();
    }

    covariance_t correlated(covariance_t::Zero());
    for (std::size_t i = 0; i < Dim; ++i)
      for (std::size_t j = 0; j < Dim; ++j) correlated(i, j) = n[p++].as<T>();

    rhs = cslibs_math::statistics::Distribution<T, Dim, lambda_ratio_exponent>{
        num, mean, correlated};
    return true;
  }
};

template <typename T, std::size_t lambda_ratio_exponent>
struct convert<
    cslibs_math::statistics::Distribution<T, 1, lambda_ratio_exponent>> {
  static Node encode(
      const cslibs_math::statistics::Distribution<T, 1, lambda_ratio_exponent>
          &rhs) {
    Node n;
    n.push_back(rhs.getN());
    n.push_back(rhs.getMean());
    n.push_back(rhs.getSquared());

    return n;
  }

  static bool decode(
      const Node &n,
      cslibs_math::statistics::Distribution<T, 1, lambda_ratio_exponent> &rhs) {
    if (!n.IsSequence() || n.size() != 3) return false;

    rhs = cslibs_math::statistics::Distribution<T, 1, lambda_ratio_exponent>(
        n[0].as<std::size_t>(), n[1].as<T>(), n[2].as<T>());
    return true;
  }
};
}  // namespace YAML

#endif  // CSLIBS_MATH_SERIALIZATION_DISTRIBUTION_HPP

#ifndef HAUSDORFF_HPP
#define HAUSDORFF_HPP

#include <cslibs_math/linear/pointcloud.hpp>
#include <cslibs_math/statistics/distribution.hpp>
#include <cslibs_math/linear/matrix.hpp>

namespace cslibs_math {
namespace linear {
template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline T hausdorff(const point_t<T,Args...> &point,
                   const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points)
{
    double h = std::numeric_limits<T>::infinity();
    for(auto &compare : points) {
        if(compare.isNormal()) {
            const T d = cslibs_math::linear::distance(point, compare);
            h = std::min(d,h);
        }
    }
    return h;
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline std::size_t nearestNeighbour(const point_t<T,Args...> &point,
                                    const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points)
{
    T min = std::numeric_limits<T>::infinity();
    std::size_t min_id = std::numeric_limits<std::size_t>::infinity();
    for(std::size_t i = 0 ; i < points.size() ; ++i) {
        const point_t<T,Args...> &compare = points.at(i);
        if(compare.isNormal()) {
            const T d = cslibs_math::linear::distance(point, compare);
            if(d < min) {
                min_id = i;
                min = d;
            }
        }
    }
    return min_id;
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline T hausdorff(const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_src,
                   const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_dst)
{
    T h = -1.0;
    for(const point_t<T,Args...> &p : points_src) {
        if(p.isNormal()) {
            const T d = hausdorff(p, points_dst);
            h = std::max(h, d);
        }
    }
    return h < 0 ? std::numeric_limits<T>::infinity() : h;
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline T hausdorffFraction(const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_src,
                           const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_dst,
                           const T max_dist)
{
    if(points_src.size() == 0)
        return 0.0;

    std::size_t accepted   = 0;
    std::size_t valid = 0;
    for(const point_t<T,Args...> &point_src : points_src) {
        if(point_src.isNormal()) {
            T h = hausdorff(point_src, points_dst);
            if(h < max_dist)
                ++accepted;
            ++valid;
        }
    }

    return valid != 0 ? accepted / static_cast<T>(valid) : 0.0;
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline T hausdorffAvg(const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_src,
                      const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_dst)
{
    if(points_src.size() == 0)
        return std::numeric_limits<T>::infinity();

    T h = 0;
    std::size_t valid = 0;
    for(const point_t<T,Args...> &point_src : points_src) {
        if(point_src.isNormal()) {
            h += hausdorff(point_src, points_dst);
            ++valid;
        }
    }
    return valid != 0 ? h / valid : std::numeric_limits<T>::infinity();
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline T hausdorffMPE(const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_src,
                      const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_dst)
{
    /// normally a product of different probabilities
    /// this yields almost always 0 ... try this little workaround

    if(points_src.size() == 0)
        return 0.0;

    T p_src = 0.0;
    std::size_t valid = 0;
    for(const point_t<T,Args...> &point_src : points_src) {
        if(point_src.isNormal()) {
            p_src += std::exp(-hausdorff(point_src, points_dst));
            ++valid;
        }
    }
    return valid != 0 ? p_src / static_cast<T>(valid) : 0.0;
}

template<typename T, template<typename,std::size_t...> typename point_t, std::size_t... Args>
inline Matrix<T, point_t<T, Args...>::Dimension, point_t<T, Args...>::Dimension>
    hausdorffCovariance(const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_src,
                        const typename cslibs_math::linear::Pointcloud<point_t<T,Args...>> &points_dst)
{
    using p_t = point_t<T, Args...>;
    static constexpr std::size_t dim_t = p_t::Dimension;

    if(points_src.size() == 0)
        return Matrix<T, dim_t, dim_t>(std::numeric_limits<T>::infinity());


    statistics::Distribution<dim_t, T> distribution;
    for(const p_t &point_src : points_src) {
        if(point_src.isNormal()) {
            std::size_t nn = nearestNeighbour(point_src, points_dst);
            if(nn == std::numeric_limits<std::size_t>::infinity())
                continue;

            const p_t pnn = points_dst.at(nn);
            distribution.add(point_src - pnn);
        }
    }

    if(distribution.getN() < 3)
        return  Matrix<T, dim_t, dim_t>(std::numeric_limits<T>::infinity()) ;

    return distribution.getCovariance();
}
}
}

#endif // HAUSDORFF_HPP

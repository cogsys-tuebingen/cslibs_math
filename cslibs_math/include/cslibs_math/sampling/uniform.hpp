#ifndef UNIFORM_SAMPLER_HPP
#define UNIFORM_SAMPLER_HPP

#include <memory>

#include <cslibs_math/random/random.hpp>
#include "traits.hpp"

namespace muse_smc {
namespace state_space_samplers {
template<typename... Types>
class Uniform {
public:
    typedef std::shared_ptr<Uniform> Ptr;

    static_assert(sizeof...(Types) > 0, "Constraint : Dimension > 0");
    static_assert(is_valid_type<Types...>::value, "Parameter list contains forbidden type!");

    static const std::size_t Dimension = sizeof...(Types);

    using rng_t    = cslibs_math::random::Uniform<Dimension>;
    using sample_t = typename rng_t::sample_t;

    Uniform() = delete;
    Uniform(const Uniform &other) = delete;

    Uniform(const sample_t &min,
            const sample_t &max,
            const unsigned int seed = 0) :
        rng_(min, max, seed)
    {
    }

    inline sample_t get()
    {
        sample_t sample = rng_.get();
        Arguments<Dimension, sample_t, Types...>::normalize(sample);
        return sample;
    }

private:
    rng_t rng_;
};
}
}

#endif /* UNIFORM_SAMPLER_HPP */

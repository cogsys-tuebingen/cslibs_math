#ifndef CSLIBS_MATH_WEIGHTED_QUATERNION_MEAN_HPP
#define CSLIBS_MATH_WEIGHTED_QUATERNION_MEAN_HPP

#include <eigen3/Eigen/Core>

namespace cslibs_math {
namespace statistics {
class EIGEN_ALIGN16 WeightedQuaternionMean {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    WeightedQuaternionMean() :
        W_(0.0),
        mean_(Eigen::Quaternion::Identity())
    {
    }

    WeightedQuaternionMean(const WeightedQuaternionMean &other) :
        W_(other.W_),
        mean_(other.mean_)
    {
    }

    WeightedQuaternionMean(WeightedQuaternionMean &&other) :
        W_(other.W_),
        mean_(std::move(other.mean_))
    {
    }

    WeightedQuaternionMean & operator += (const Eigen::Quaternion &q, const double w)
    {
        if(W_ == 0.0) {
            mean = q * w;
            W_ = w;
        } else {
            const double W_1 = W;
            W_ += w;

            if(quaternionsClose(mean, q)) {
                mean_ = (mean_ / W_1 + q * w) / W_;
            } else {
                mean_ = (mean_ / W_1 + q.inverse() * w) / W_;
            }
        }
    }

private:
    double         W_1_;
    double         W_;
    Eigen::Quaternion mean_;


    //Returns true if the two input quaternions are close to each other. This can
    //be used to check whether or not one of two quaternions which are supposed to
    //be very similar but has its component signs reversed (q has the same rotation as
    //-q)
    inline bool quaternionsClose(const Eigen::Quaternion &q_a,
                                 const Eigen::Quaternion &q_b)
    {
        double dot = q_a.dot(q_b);
        return dot < 0.0;
    }

};
}
}
#endif // CSLIBS_MATH_WEIGHTED_QUATERNION_MEAN_HPP

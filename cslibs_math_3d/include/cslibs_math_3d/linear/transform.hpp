#ifndef CSLIBS_MATH_3D_TRANSFORM_3D_HPP
#define CSLIBS_MATH_3D_TRANSFORM_3D_HPP

#include <cslibs_math_3d/linear/quaternion.hpp>
#include <cslibs_math_3d/linear/vector.hpp>
#include <cslibs_math/common/angle.hpp>

namespace cslibs_math_3d {
class Transform3d {
public:
    using eigne_vector_6d_t = Eigen::Matrix<double, 6, 1>;

    inline Transform3d()
    {
    }

    static inline Transform3d identity()
    {
        return Transform3d();
    }

    inline Transform3d(const double x,
                       const double y,
                       const double z) :
        translation_(x, y, z)
    {
    }

    inline Transform3d(const Vector3d &translation) :
        translation_(translation)
    {
    }

    inline Transform3d(const double yaw) :
        rotation_(yaw)
    {
    }

    inline Transform3d(const double x,
                       const double y,
                       const double z,
                       const double yaw) :
        translation_(x, y, z),
        rotation_(yaw)
    {
    }

    inline Transform3d(const Vector3d &translation,
                       const double yaw) :
        translation_(translation),
        rotation_(yaw)
    {
    }

    inline Transform3d(const Transform3d &other) :
        translation_(other.translation_),
        rotation_(other.rotation_)
    {
    }

    inline Transform3d(Transform3d &&other) :
        translation_(other.translation_),
        rotation_(other.rotation_)
    {
    }

    inline Vector3d operator * (const Vector3d &v) const
    {
        return rotation_ * v + translation_;
    }

    inline Transform3d operator * (const Transform3d &other) const
    {
        return Transform3d(rotation_ * other.translation_, rotation_ * other.rotation_);
    }

    inline Transform3d & operator *= (const Transform3d &other)
    {
        translation_ = (*this) * other.translation_;
        rotation_ *= other.rotation_;
        return *this;
    }

    inline Transform3d& operator = (const Transform3d &other)
    {
        translation_ = other.translation_;
        rotation_ = other.rotation_;
        return *this;
    }


    inline Transform3d& operator = (const Eigen::Matrix<double, 6, 1> &eigen)
    {
        translation_ = Vector3d(eigen(0), eigen(1), eigen(2));
        rotation_    = Quaternion(eigen(3), eigen(4), eigen(5));
        return *this;
    }

    inline Transform3d& operator = (Transform3d &&other)
    {
        translation_ = std::move(other.translation_);
        rotation_    = std::move(other.rotation_);
        return *this;
    }

    inline Transform3d inverse() const
    {
        Quaternion rotation_inverse = rotation_.invert();
        return Transform3d(rotation_inverse * translation_,
                           rotation_inverse);
    }

    inline Transform3d operator -() const
    {
        return inverse();
    }

    inline double & tx()
    {
        return translation_(0);
    }

    inline double tx() const
    {
        return translation_(0);
    }

    inline double & ty()
    {
        return translation_(1);
    }

    inline double ty() const
    {
        return translation_(1);
    }

    inline double tz() const
    {
        return translation_(1);
    }

    inline double roll() const
    {
        return rotation_.roll();
    }

    inline double pitch() const
    {
        return rotation_.pitch();
    }

    inline double yaw() const
    {
        return rotation_.yaw();
    }

    inline Vector3d & translation()
    {
        return translation_;
    }

    inline Vector3d const & translation() const
    {
        return translation_;
    }

    inline void setYaw(const double yaw)
    {
        rotation_.setYaw(yaw);
    }

    inline eigne_vector_6d_t toEigen() const
    {
        return cslibs_math::linear::eigen::create<eigne_vector_6d_t>(translation_(0), translation_(1), translation_(2),
                                                                     rotation_.roll(), rotation_.pitch(), rotation_.yaw());
    }

    inline void setFrom(const Eigen::Matrix<double, 6, 1> &eigen)
    {
        translation_(0) = eigen(0);
        translation_(1) = eigen(1);
        translation_(2) = eigen(2);
        rotation_.setRPY(eigen(3),eigen(4),eigen(5));
    }

    inline void setFrom(const double x,
                        const double y,
                        const double z,
                        const double roll,
                        const double pitch,
                        const double yaw)
    {
        translation_(0) = x;
        translation_(1) = y;
        translation_(2) = z;
        rotation_.setRPY(roll, pitch, yaw);
    }

    inline Transform3d interpolate(const Transform3d &other,
                                   const double ratio) const
    {
        assert(ratio  >= 0.0);
        assert(ratio <= 1.0);
        if(ratio == 0.0) {
            return *this;
        }
        if(ratio == 1.0) {
            return other;
        }

        const  double ratio_inverse = 1.0 - ratio;
        const  Vector3d translation = translation_ * ratio_inverse + other.translation_ * ratio;
        const  Quaternion  rotation = rotation_.interpolate(other.rotation_, ratio);
        return Transform3d(translation, rotation);
    }

private:
    inline Transform3d(const Vector3d &translation,
                       const Quaternion &rotation) :
        translation_(translation),
        rotation_(rotation)
    {
    }

    Vector3d    translation_;
    Quaternion  rotation_;
} __attribute__ ((aligned (64)));
}

inline std::ostream & operator << (std::ostream &out, const cslibs_math_3d::Transform3d &t)
{
    out << "[" << t.tx() << "," << t.ty() << "," << t.roll() << "," << t.pitch() << "," << t.yaw() << "]";
    return out;
}

#endif // CSLIBS_MATH_3D_TRANSFORM_3D_HPP
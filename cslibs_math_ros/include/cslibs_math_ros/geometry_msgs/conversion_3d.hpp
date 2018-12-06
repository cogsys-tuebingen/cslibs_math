#ifndef CSLIBS_MATH_ROS_GEOMETRY_MSGS_CONVERSION_3D_HPP
#define CSLIBS_MATH_ROS_GEOMETRY_MSGS_CONVERSION_3D_HPP
#include <tf/tf.h>
#include <geometry_msgs/Point.h>
#include <cslibs_math_3d/linear/vector.hpp>

namespace cslibs_math_ros {
namespace geometry_msgs {
namespace conversion_3d {
inline cslibs_math_3d::Vector3d from(const ::geometry_msgs::Point &p)
{
    return cslibs_math_3d::Vector3d(p.x, p.y, p.z);
}

inline ::geometry_msgs::Point toPoint(const ::cslibs_math_3d::Vector3d &p)
{
    ::geometry_msgs::Point res;
    res.x = p(0);
    res.y = p(1);
    res.z = p(2);
    return res;
}

inline ::geometry_msgs::Vector3 toVector3(const ::cslibs_math_3d::Vector3d &p)
{
    ::geometry_msgs::Vector3 res;
    res.x = p(0);
    res.y = p(1);
    res.z = p(2);
    return res;
}

}
}
}
#endif // CSLIBS_MATH_ROS_GEOMETRY_MSGS_CONVERSION_3D_HPP

cmake_minimum_required(VERSION 2.8.3)
project(cslibs_math_3d)

find_package(catkin REQUIRED COMPONENTS
  cslibs_math
)

find_package(catkin REQUIRED COMPONENTS
    cslibs_math
)
find_package(Eigen3 REQUIRED)

catkin_package(
    INCLUDE_DIRS    include
    CATKIN_DEPENDS  cslibs_math
    DEPENDS         Eigen3
)

include_directories(
    ${Eigen3_INCLUDE_DIR}
    ${catkin_INCLUDE_DIRS}
    include
)

cslibs_math_3d_show_headers()

#### test
find_package(tf QUIET)
if(${tf_FOUND})
    include_directories(
        ${tf_INCLUDE_DIRS}
    )
    cslibs_math_3d_add_unit_test_gtest(${PROJECT_NAME}_test_quaternion
        SRCS test/quaternion.cpp
    )
    cslibs_math_add_unit_test_gtest(${PROJECT_NAME}_test_transform
        SRCS test/transform.cpp
    )
#    cslibs_math_add_unit_test_gtest(${PROJECT_NAME}_test_vector
#        SRCS test/vector.cpp
#    )
endif()

install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
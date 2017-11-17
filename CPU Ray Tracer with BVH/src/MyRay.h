#ifndef MY_RAY_H
#define MY_RAY_H

#include <Eigen/Geometry>
#include <cfloat>

//class for the ray we're shooting
class MyRay
{

public:
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;

    //we need to add this variable for supersampling shit, so we can use the raw direction to get our new samples
    //and this normalized direction for the ray tracing
    Eigen::Vector3f normalizedDirect;
    int superSampleDepth;

    int depth;
    bool isInObject;

    MyRay();

    MyRay(Eigen::Vector3f origin, Eigen::Vector3f direct, Eigen::Vector3f normalDirect, int d, int superDepth);

    void Equals(MyRay ray);

};

#endif

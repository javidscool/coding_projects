#include "MyRay.h"

MyRay::MyRay()
{
    origin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    direction = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    normalizedDirect = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    depth = -1;
    superSampleDepth = -1;
    isInObject = false;
}

MyRay::MyRay(Eigen::Vector3f orgin, Eigen::Vector3f direct, Eigen::Vector3f normalDirect, int d, int superDepth)
{
    origin = orgin;
    direction = direct;
    normalizedDirect = normalDirect;
    depth = d;
    superSampleDepth = superDepth;
    isInObject = false;
}

void MyRay::Equals(MyRay ray)
{
    origin = ray.origin;
    direction = ray.direction;
    normalizedDirect = ray.normalizedDirect;
    depth = ray.depth;
}

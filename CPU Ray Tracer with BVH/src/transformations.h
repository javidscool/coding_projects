#ifndef VIRTUOSO_TRANSFORMATIONS_H
#define VIRTUOSO_TRANSFORMATIONS_H


#include <Eigen/Geometry>

Eigen::Matrix4f projection(float fov = 3.14159 / 4, float aspect = 4.0/3.0, float zNear = .01, float zFar = 100 );



Eigen::Matrix4f rotateY(float theta);



Eigen::Matrix4f rotateZ(float theta);



Eigen::Matrix4f rotateX(float radians);


Eigen::Matrix4f scale(float x, float y, float z);


Eigen::Matrix4f translation(float x, float y, float z);

#endif

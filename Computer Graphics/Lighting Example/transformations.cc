#include "transformations.h"

 Eigen::Matrix4f projection(float fov , float aspect , float zNear, float zFar){

Eigen::Matrix4f temp;

float f = 1.0  / tan(fov / 2.0);

temp<<f/aspect, 0, 0, 0,
      0, f, 0, 0,
      0, 0, (zFar + zNear) / (zNear - zFar),2 * zFar * zNear / (zNear - zFar),
      0, 0, -1, 0;

return temp;
}



 Eigen::Matrix4f rotateY(float theta)
{
    Eigen::Matrix4f temp;


    temp<<cos(theta), 0, sin(theta), 0,
    0, 1, 0, 0,
    -sin(theta), 0, cos(theta), 0,
    0, 0, 0, 1;

    return temp;

}



Eigen::Matrix4f rotateZ(float theta)
{

    Eigen::Matrix4f temp;

    temp<<cos(theta), -sin(theta), 0, 0,
    sin(theta), cos(theta), 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1;

    return temp;
}



 Eigen::Matrix4f rotateX(float radians)
{

    Eigen::Matrix4f temp;

    temp<<1, 0, 0, 0,
    0, cos(radians), -sin(radians), 0,
    0, sin(radians), cos(radians), 0,
    0, 0, 0, 1;
    return temp;

}





 Eigen::Matrix4f scale(float x, float y, float z)
{
    Eigen::Matrix4f temp;

    temp<<x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1;
    return temp;
}



Eigen::Matrix4f translation(float x, float y, float z)
{

    Eigen::Matrix4f temp;

    temp<<1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;

    return temp;

}

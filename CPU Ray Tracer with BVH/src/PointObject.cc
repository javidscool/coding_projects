#include "PointObject.h"
#include <fstream>
#include <se/graphics/image/formats/LdrImage.h>


PointObject::PointObject(std::vector<Point> pt, Eigen::Matrix4f mMatrix, Eigen::Vector3f col)
{
    pointList = pt;

    color = col;

    //converting positions to eye space
    std::vector<Point> temp;

    modelMatrix = mMatrix;

    //putting all triangle point coordinates and normals from object space to camera space
    for(int i = 0; i < pt.size(); i++){
        Point point;

        //converting triangle coordinates for this mesh from object space to camera space
        point.Equals(ConvertToCamera(pt[i], modelMatrix));

        //now put this transformed-to-cam-space triangle into my triangleList
        temp.push_back(point);
    }

    pointList = temp;

    //initializing the bounding box
    AABBox bbox(pointList[0].position, pointList[1].position, pointList[2].position);

    //creating the rest of the bounding box for this object
    for(int i = 3; i < pointList.size(); i++){
        bbox = Union(bbox, pointList[i].position);
    }

    //now set the bounding box
    boundingBox = bbox;

}

//this transformed all points in the triangle from object space to eye space
Point PointObject::ConvertToCamera(Point pt, Eigen::Matrix4f modelView){

    Eigen::Matrix<float, 4, 1> trans;
    Eigen::Matrix<float, 4, 1> ans;


    trans <<    pt.position[0],
                pt.position[1],
                pt.position[2],
                1.0f;

    ans = modelView * trans;

    pt.position[0] = ans(0,0);
    pt.position[1] = ans(1,0);
    pt.position[2] = ans(2,0);


    return pt;
}


PointObject::PointObject()
{

}


std::vector<Point> & PointObject::GetPoints()
{
    return pointList;
}


//use this constructor if you're point tracing a sphere
PointObject::PointObject(Eigen::Vector3f col, float x, float y, float z, float radius){
    color = col;

    std::vector<Eigen::Vector3f> spherePoints = sphereSamplePoints(x, y, z, radius, (7/8)*(3.14), (3.14), 0.0f, 3.14/8, 3000);
    //std::vector<Eigen::Vector3f> spherePoints = sphereSamplePoints(x, y, z, radius, 0.0f, (2*3.14), 0.0f, 3.14f, 15000);

    //getting these sphere coordinates into my pointList
    for(int i = 0; i < spherePoints.size(); i++){
        Point point;

        //converting triangle coordinates for this mesh from object space to camera space
        point.Equals(spherePoints[i]);

        //now put this transformed-to-cam-space triangle into my triangleList
        pointList.push_back(point);
    }

    //initializing the bounding box
    AABBox bbox(pointList[0].position, pointList[1].position, pointList[2].position);

    //creating the rest of the bounding box for this object
    for(int i = 3; i < pointList.size(); i++){
        bbox = Union(bbox, pointList[i].position);
    }

    //now set the bounding box
    boundingBox = bbox;
}

Eigen::Vector3f PointObject::SetCentroid(){
    Eigen::Vector3f temp(0.0f, 0.0f, 0.0f);

    for(int i = 0; i < pointList.size(); i++){
        temp += pointList[i].position;
    }

    temp = temp / pointList.size();

    return temp;
}

//angles are in radians
std::vector<Eigen::Vector3f> PointObject::sphereSamplePoints(float x, float y, float z, float radius,
        float thetaMin, float thetaMax,
        float phiMin, float phiMax, unsigned int sampleCount)
{

    std::vector<Eigen::Vector3f> samples;


    for(unsigned int i = 0; i < sampleCount; i++) {

        float uniform1 = (float)rand() / (float)RAND_MAX;
        float uniform2 = (float)rand() / (float)RAND_MAX;

        float thetaRand = uniform1 * (thetaMax - thetaMin) + thetaMin;
        float phiRand = uniform2 * (phiMax - phiMin) + phiMin;

        float sx = (radius * sin(phiRand) * cos(thetaRand)) +x;
        float sz = (radius * sin(phiRand) * sin(thetaRand))  +z;
        float sy = (radius * cos(phiRand)) +y ;

        samples.push_back(Eigen::Vector3f(sx,sy,sz));


    }

    log << "number of points that Chris gives me: " << samples.size() << endl;

    return samples;
}





#ifndef POINT_OBJECT_H
#define POINT_OBJECT_H

#include "MyRay.h"
#include "Mesh.h"
#include "AABBox.h"
#include<se/core/shared_ptr.h>
#include<se/graphics/image/FloatImageRGBA.h>

#include <iostream>
#include <se/core/Log.h>
#define log se::core::Log::status()

using namespace std;

class PointObject{
    private:
        Point ConvertToCamera(Point pt, Eigen::Matrix4f modelView);

        std::vector<Eigen::Vector3f> sphereSamplePoints(float x, float y, float z, float radius,
        float thetaMin, float thetaMax,
        float phiMin, float phiMax, unsigned int sampleCount);

        Eigen::Matrix4f modelMatrix;


    public:
        Eigen::Vector3f SetCentroid();
        std::vector<Point> pointList;
        std::vector<Point> modPointList;
        AABBox boundingBox;

        //Eigen::Vector3f masterCentroid;

        Eigen::Vector3f color;

        PointObject(std::vector<Point> pt, Eigen::Matrix4f mMatrix, Eigen::Vector3f col);

        //use this constructor if you're point tracing a sphere
        PointObject(Eigen::Vector3f col, float x, float y, float z, float radius);

        PointObject();

        std::vector<Point> & GetPoints();
};

#endif


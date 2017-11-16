#ifndef POINT_TRACER_H
#define POINT_TRACER_H

#include <Eigen/Geometry>
#include <Eigen/Core>
#include <Eigen/QR>

#include <cmath>
#include "Mesh.h"
#include "MyRay.h"
#include "PointObject.h"
#include "Misc.h"
#include "AABBox.h"
#include<se/core/shared_ptr.h>
#include <st/image/pano/LatitudeLongitude.h>
#include <cfloat>
#include <iostream>
#include <se/core/Log.h>
#define log se::core::Log::status()

using namespace std;


class PointTracer{
    private:
        //znear distance = 0.01
        //float fov = 3.14159 / 4, float aspect = 4.0/3.0, float zNear = .01, float zFar = 100

        std::vector<PointObject> objectList;

        float uL_x;
        float uL_y;
        float height_of_pixel;
        float width_of_pixel;

        float window_width;
        float window_height;

        float near_plane;


        std::vector<DirLight> directionalLights;

        std::vector<PointLight> pointLights;

        std::shared_ptr<st::image::pano::LatitudeLongitude> background;



        //void SetCoordSystem(PointObject object, Eigen::Vector3f centroid, float h);

        //void CreateCurve(PointObject object, Eigen::Vector3f centroid, float h);

        //float IsHit(MyRay testRay);

        MyRay GetLocalRay(MyRay ray);




    public:

        PointTracer(std::vector<PointObject> objList, const int width, const int height);

        Color GetColor(MyRay ray);

        MyRay GetOriginRay(int j, int i);

        //returns to me the t of the intersection
        float IntersectBoundingBox(const AABBox &bounds, Eigen::Vector3f invDir, Eigen::Vector3i dirIsNeg, Eigen::Vector3f rayOrig, float rayMaxt);

        bool IsInBox(const AABBox &bounds, Eigen::Vector3f pt);


        Eigen::Vector3f GetIntPt(MyRay testRay, float t);


        float Distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2);


        void SetCoordSystem(PointObject object, Eigen::Vector3f centroid, float h);

        void CreateCurve(PointObject &object, Eigen::Vector3f centroid, float h);

        std::vector<Eigen::Vector3f> GetCurvePoints(PointObject &object, Eigen::Vector3f centroid);

        float IsHit(MyRay testRay);


        std::vector<Eigen::Vector3f> GetRayPoints(PointObject &object, MyRay ray);

        bool hitSuccessB;


        //our coord system axes
        Eigen::Vector3f zAxis;
        Eigen::Vector3f yAxis;
        Eigen::Vector3f xAxis;

        Eigen::Matrix<float, 6, 1> curve;

};


#endif


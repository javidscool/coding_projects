#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <Eigen/Geometry>
#include <cmath>
#include "Mesh.h"
#include "MyRay.h"
#include "Object.h"
#include "Misc.h"
#include<se/core/shared_ptr.h>
#include <st/image/pano/LatitudeLongitude.h>
#include <cfloat>
#include <iostream>
#include <se/core/Log.h>
#define log se::core::Log::status()

using namespace std;


class RayTracer{
    private:
        //znear distance = 0.01
        //float fov = 3.14159 / 4, float aspect = 4.0/3.0, float zNear = .01, float zFar = 100

        std::vector<Object> objectList;

        float uL_x;
        float uL_y;
        float height_of_pixel;
        float width_of_pixel;

        float window_width;
        float window_height;

        float near_plane;

        ABGTemp baryTemp;

        float alpha, beta, gamma;

        float invUV[2];

        std::vector<DirLight> directionalLights;

        std::vector<PointLight> pointLights;

        std::shared_ptr<st::image::pano::LatitudeLongitude> background;


    public:

        ofstream fout;

        RayTracer(std::vector<Object> objList, const int width, const int height, std::vector<Eigen::Matrix4f> modelViews);

        Color GetColor(MyRay ray, float tMin, float tTemp, Triangle hitTri);

        MyRay GetOriginRay(int j, int i);

        MyRay GetReflectionRay(MyRay incidentRay, Eigen::Vector4f normal, Eigen::Vector3f origin, Eigen::Vector3f hitNorToTri);


        MyRay GetRefractionRay(MyRay incidentRay, Eigen::Vector3f normal, Eigen::Vector3f origin, Object hitObject, Eigen::Vector3f hitNorToTri);

        float Snell(float index1, float index2, float angle);


        //shit for adaptive supersampling
        Color AdaptiveSuperSamp(MyRay originRay, float tMin, float tTemp, Triangle hitTri, int j, int i);

        std::vector<MyRay> GetAdapRays(MyRay middleRay, int j, int i);

        bool IsColorTooDiff(Color color1, Color color2);

        Color AdaptiveSuperPass2(MyRay ray1, MyRay ray2, Color ray1Col, Color ray2Col, int situation, float tMin, float tTemp, Triangle hitTri);

        std::vector<MyRay> GetAdapRaysPass2(MyRay ray1, MyRay ray2, int situation, int superSampMin);

        //shit for adaptive supersampling




        MyRay GetPtShadowRay(Eigen::Vector3f intersection_pt, PointLight ptLight);

        MyRay GetDirShadowRay(Eigen::Vector3f intersection_pt, DirLight dirLight);

        bool IsInShadowPt(MyRay ptShadowRay);

        bool IsInShadowDir(MyRay dirShadowRay);


        float IsHit(Triangle tri, MyRay testRay);

        float IsGenericHit(Triangle tri, MyRay testRay);

        Triangle ConvertToCamera(Triangle tri, Eigen::Matrix4f modelview);

        Triangle ConvertNormToCamera(Triangle tri, Eigen::Matrix4f modelview);

        Eigen::Vector3f GetIntPt(MyRay testRay, float t);

        Eigen::Vector3f GetNormalToTri(Triangle tri);

        void GetHitUV(Triangle hitTri);

        Eigen::Vector4f GetHitNormal(Triangle hitTri);

        Eigen::Vector3f GetPoint(Triangle hitTri);

        double Distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2);


};


#endif

#ifndef LIGHTMAP_H
#define LIGHTMAP_H

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


class Lightmap{
    private:

        ABGTemp baryTemp;

        float alpha, beta, gamma;

        float invUV[2];

        std::vector<DirLight> directionalLights;

        std::vector<PointLight> pointLights;


    public:

        std::vector<Object> objectList;

        Lightmap(std::vector<Object> objList);

        Color GetColor(Object curObj, float v, float u);



        MyRay GetPtShadowRay(Eigen::Vector3f intersection_pt, PointLight ptLight);

        MyRay GetDirShadowRay(Eigen::Vector3f intersection_pt, DirLight dirLight);

        bool IsInShadowPt(MyRay ptShadowRay);

        bool IsInShadowDir(MyRay dirShadowRay);


        bool IsUVHit(float v, float u, Triangle tri);

        float IsGenericHit(Triangle tri, MyRay testRay);

        Eigen::Vector3f GetNormalToTri(Triangle tri);

        void GetHitUV(Triangle hitTri);

        Eigen::Vector4f GetHitNormal(Triangle hitTri);

        Eigen::Vector3f GetPoint(Triangle hitTri);

};


#endif

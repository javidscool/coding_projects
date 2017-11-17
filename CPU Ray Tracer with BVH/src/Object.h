#ifndef OBJECT_H
#define OBJECT_H

#include "MyRay.h"
#include "Mesh.h"
#include<se/core/shared_ptr.h>
#include<se/graphics/image/FloatImageRGBA.h>

#include <iostream>
#include <se/core/Log.h>
#define log se::core::Log::status()

using namespace std;

class Object{
    private:
        Triangle ConvertToCamera(Triangle tri, Eigen::Matrix4f modelView);

        Triangle ConvertNormToCamera(Triangle tri, Eigen::Matrix3f normalMat);

        Eigen::Matrix4f modelMatrix;
        Eigen::Matrix3f normalMatrix;

    public:
        std::vector<Triangle> triangleList;
        float indexOfRefrac;
        float reflection_coeff;
        float refraction_coeff;
        bool hasText;
        std::shared_ptr<se::graphics::image::FloatImageRGBA> texture;



        Object(std::vector<Triangle> tr, Eigen::Matrix4f mMatrix, Eigen::Matrix3f nMatrix, float idx, float refl, float refrac, const std::string & fileName);

        Object();


        std::vector<Triangle> & GetTriangles(const MyRay& ray);
        std::vector<Triangle> & GetTriangles();
        void ToggleTexture();

};

#endif

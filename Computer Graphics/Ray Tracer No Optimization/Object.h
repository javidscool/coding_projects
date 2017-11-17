#ifndef OBJECT_H
#define OBJECT_H

#include "MyRay.h"
#include "Mesh.h"
#include<se/core/shared_ptr.h>
#include<se/graphics/image/FloatImageRGBA.h>

class Object{
    public:
        std::vector<Triangle> triangleList;
        float indexOfRefrac;
        float reflection_coeff;
        float refraction_coeff;
        bool hasText;
        std::shared_ptr<se::graphics::image::FloatImageRGBA> texture;



        Object(std::vector<Triangle> tr, float idx, float refl, float refrac, const std::string & fileName);

        Object();


        std::vector<Triangle> & GetTriangles(const MyRay& ray);
        void ToggleTexture();

};

#endif

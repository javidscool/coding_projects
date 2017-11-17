#include "Object.h"
#include <fstream>
#include <se/graphics/image/formats/LdrImage.h>


Object::Object(std::vector<Triangle> tr, float idx, float refl, float refrac, const std::string & fileName)
{
    triangleList = tr;
    indexOfRefrac = idx;
    reflection_coeff = refl;
    refraction_coeff = refrac;

    //setting up the texture for this obj
    std::ifstream text(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
    se::graphics::image::formats::LdrImage li(text);
    texture.reset(new se::graphics::image::FloatImageRGBA(li));

    hasText = true;

}


Object::Object()
{
    indexOfRefrac = 0.0f;
    reflection_coeff = 0.0f;
    refraction_coeff = 0.0f;

    hasText = false;

}


std::vector<Triangle> & Object::GetTriangles(const MyRay & ray)
{
    return triangleList;
}

void Object::ToggleTexture(){
    hasText = !hasText;
}

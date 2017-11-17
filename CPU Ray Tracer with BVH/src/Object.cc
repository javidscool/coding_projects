#include "Object.h"
#include <fstream>
#include <se/graphics/image/formats/LdrImage.h>


Object::Object(std::vector<Triangle> tr, Eigen::Matrix4f mMatrix, Eigen::Matrix3f nMatrix, float idx, float refl, float refrac, const std::string & fileName)
{
    triangleList = tr;
    indexOfRefrac = idx;
    reflection_coeff = refl;
    refraction_coeff = refrac;

    //setting up the texture for this obj
    std::ifstream text(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
    se::graphics::image::formats::LdrImage li(text);
    texture.reset(new se::graphics::image::FloatImageRGBA(li));

    text.close();

    hasText = true;


    //converting positions and normals to eye space
    std::vector<Triangle> temp;

    modelMatrix = mMatrix;
    normalMatrix = nMatrix;

    //putting all triangle point coordinates and normals from object space to camera space
    for(int i = 0; i < tr.size(); i++){
        Triangle tri;

        //converting triangle coordinates for this mesh from object space to camera space
        tri.Equals(ConvertToCamera(tr[i], modelMatrix));

        //this transformed all normals of the triangle from object space to camera space
        tri.Equals(ConvertNormToCamera(tri, normalMatrix));

        //now put this transformed-to-cam-space triangle into my triangleList
        temp.push_back(tri);
    }

    triangleList = temp;

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

std::vector<Triangle> & Object::GetTriangles()
{
    return triangleList;
}

void Object::ToggleTexture(){
    hasText = !hasText;
}


//this transformed all points in the triangle from object space to eye space
Triangle Object::ConvertToCamera(Triangle tri, Eigen::Matrix4f modelView){

    Eigen::Matrix<float, 4, 1> trans;
    Eigen::Matrix<float, 4, 1> ans;

    for(int i = 0; i < 3; i++)
    {
        trans <<    tri.vertices[i].position[0],
                    tri.vertices[i].position[1],
                    tri.vertices[i].position[2],
                    1.0f;

        ans = modelView * trans;

        tri.vertices[i].position[0] = ans(0,0);
        tri.vertices[i].position[1] = ans(1,0);
        tri.vertices[i].position[2] = ans(2,0);

    }
    return tri;
}


//this transformed all normals of the triangle from object space to eye space
Triangle Object::ConvertNormToCamera(Triangle tri, Eigen::Matrix3f normalMat){
    Eigen::Matrix<float, 3, 1> trans;
    Eigen::Matrix<float, 3, 1> ans;

    for(int i = 0; i < 3; i++)
    {
        trans <<    tri.vertices[i].normal[0],
                    tri.vertices[i].normal[1],
                    tri.vertices[i].normal[2];

        ans = normalMat * trans;

        tri.vertices[i].normal[0] = ans(0,0);
        tri.vertices[i].normal[1] = ans(1,0);
        tri.vertices[i].normal[2] = ans(2,0);

    }
    return tri;
}

#ifndef MISC_H
#define MISC_H

#include <Eigen/Geometry>
#include <cmath>
#include "Mesh.h"
#include "MyRay.h"
#include "Object.h"
#include<se/core/shared_ptr.h>
#include <st/image/pano/LatitudeLongitude.h>
#include <cfloat>
#include <iostream>
#include <se/core/Log.h>
#define log se::core::Log::status()

using namespace std;


struct Color{
    float redComp;
    float greenComp;
    float blueComp;

    Color(){
        redComp = 0.0f;
        greenComp = 0.0f;
        blueComp = 0.0f;
    }

    Color(float red, float green, float blue){
        redComp = red;
        greenComp = green;
        blueComp = blue;
    }

    //we need to overload the division operator for adaptive supersampling
    Color operator/(float num){
        Color newColor;

        newColor.redComp = redComp / num;
        newColor.greenComp = greenComp / num;
        newColor.blueComp = blueComp / num;

        return newColor;
    }

    //we need to overload the mult operator for recursive getcolor calls when you mult by reflective/refractive coeff.
    Color operator*(float num){
        Color newColor;

        newColor.redComp = redComp * num;
        newColor.greenComp = greenComp * num;
        newColor.blueComp = blueComp * num;

        return newColor;
    }

    //we need to overload the plus operator for recursive getcolor calls when you mult by reflective/refractive coeff.
    Color operator+(const Color myColor){
        Color newColor;

        newColor.redComp = redComp + myColor.redComp;
        newColor.greenComp = greenComp + myColor.greenComp;
        newColor.blueComp = blueComp + myColor.blueComp;

        return newColor;
    }

    //we need to overload the mult operator for recursive getcolor calls when you mult by reflective/refractive coeff.
    Color & operator*=(float num){

        redComp *= num;
        greenComp *= num;
        blueComp *= num;

        return *this;
    }

    //we need to overload the plus operator for recursive getcolor calls when you mult by reflective/refractive coeff.
    Color & operator+=(const Color myColor){

        redComp += myColor.redComp;
        greenComp += myColor.greenComp;
        blueComp += myColor.blueComp;

        return *this;
    }

};

struct ABGTemp{
    float alphaTemp;
    float betaTemp;
    float gammaTemp;

    ABGTemp(){
        alphaTemp = 0.0f;
        betaTemp = 0.0f;
        gammaTemp = 0.0f;
    }

    void Reset(){
        alphaTemp = 0.0f;
        betaTemp = 0.0f;
        gammaTemp = 0.0f;
    }
};

//struct for point lights
struct PointLight{
    Eigen::Vector3f lightPos;

    //colors are between 0 and 1
    Color lightColor;

    PointLight(Eigen::Vector3f lpos, Color lghtColor){
        lightPos = lpos;
        lightColor = lghtColor;
    }
};

//struct for directional lights
struct DirLight{
    Eigen::Vector4f dirLightL;

    //colors are between 0 and 1
    Color lightColor;

    DirLight(Eigen::Vector4f dirL, Color lghtColor){
        dirLightL = dirL;
        lightColor = lghtColor;
    }
};

#endif

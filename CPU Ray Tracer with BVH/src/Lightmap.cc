#include "Lightmap.h"
#include<se/graphics/image/formats/RgbeImage.h>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace se::graphics::image;

//ray-tracer constructor
Lightmap::Lightmap(std::vector<Object> objList)
{
    std::vector<Triangle> temp;

    //set the object list
    objectList = objList;

    //initializing (u,v) to bunk values
    invUV[0] = -1.0f;
    invUV[1] = -1.0f;


    //adding some directional lights (these coordinates are pointing twords the directional light)
    directionalLights.push_back(  DirLight(Eigen::Vector4f(3.0f, -4.0f, 1.0f, 0.0f), Color(1.0f, 0.0f, 0.0f)) );
    //directionalLights.push_back(  DirLight(Eigen::Vector4f(0.0f, 0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 1.0f)) );

    //adding some point lights (these coordinates are the POSITIONS of the point lights)
    //this point light is to the left of the robot
    pointLights.push_back( PointLight(Eigen::Vector3f(-2.0f, 0.0f, -9.0f), Color(0.0f, 1.0f, 0.0f)) );


    log << endl;
    log << "Number of directional lights: " << directionalLights.size() << endl;
    log << "Number of point lights: " << pointLights.size() << endl;
}



//MAIN FUNCTION THAT DOES EVERYTHING
Color Lightmap::GetColor(Object curObj, float v, float u)
{
    Triangle hitTri;

    Color resultColor;

    bool isHit = false;

    //iterating through all the triangles in the trianglelist
    for(int i = 0; i < curObj.triangleList.size(); i++)
    {

        //get a potential alpha-beta-gamma
        isHit = IsUVHit(v, u, curObj.triangleList[i]);

        //if we hit a triangle using the potential UV coords
        if(isHit)
        {
            //log << "I got a hit" << endl;

            //i'm assuming that the first triangle I hit is the triangle that has the intersection point that
            //I want (i'm assuming a 1-to-1 mapping...so no tiling)
            alpha = baryTemp.alphaTemp;
            beta = baryTemp.betaTemp;
            gamma = baryTemp.gammaTemp;


            //log << "alpha when I just hit: " << alpha << endl;
            //log << "beta when I just hit: " << beta << endl;
            //log << "gamma when I just hit: " << gamma << endl;

            //get the triangle that was hit
            hitTri.Equals(curObj.triangleList[i]);

            break;
        }

    }


    //if we hit something using the UV's
    if(isHit)
    {

        //...get the intersection
        Eigen::Vector3f intersectionPt = GetPoint(hitTri);


        //getting the hit UV
        GetHitUV(hitTri);

        //log << "u: " << u << endl;
        //log << "v: " << v << endl;

        //log << "invUV[0]: " << invUV[0] << endl;
        //log << "invUV[1]: " << invUV[1] << endl;


        //get normal of intersection point on hit triangle
        Eigen::Vector4f hitNormal = GetHitNormal(hitTri);




        //////////////////////////////////////////
        /////precomputing stuff for lighting//////
        //////////////////////////////////////////

        //getting surface normal to the triangle that I hit
        Eigen::Vector3f hitNorToTri = GetNormalToTri(hitTri);

        //moving the intersection point up 1e-3 times the hit normal direction
        Eigen::Vector3f modifiedIntPt = intersectionPt + (1e-3 * hitNorToTri);

        //////////////////////////////////////////
        /////precomputing stuff for lighting//////
        //////////////////////////////////////////

        MyRay pointShadowRay;
        //first do shadowing with point lights
        for(int ptLight = 0; ptLight < pointLights.size(); ptLight++){

            //doing a deep copy
            pointShadowRay.Equals( GetPtShadowRay(modifiedIntPt, pointLights[ptLight]) );

            //if we're NOT in shadow from the point light
            if( !(IsInShadowPt(pointShadowRay)) ){

                //normalizing the point lights direction so we can do n dot L
                pointShadowRay.direction.normalize();


                Eigen::Vector4f LPt;

                LPt.head<3>() = pointShadowRay.direction;
                LPt[3] = 0.0f;


                float nDotLPt = hitNormal.dot(LPt);

                if(nDotLPt < 0.0f){
                    nDotLPt = 0.0f;
                }

                //if you want the lightmap to contain texture information
                if(curObj.hasText){
                    //indexing into the texture
                    float xText = invUV[0] * (float)curObj.texture->width;
                    float yText = invUV[1] * (float)curObj.texture->height;

                    se::math::Vector3D textCoord(xText, yText, 0.0f);

                    se::graphics::image::Color4f col = curObj.texture->read(textCoord, FloatImageRGBA::LINEAR);

                    //log << "u: " << u << endl;
                    //log << "v: " << v << endl;

                    //adding n dot L to the color components
                    resultColor.redComp += nDotLPt * col.x * pointLights[ptLight].lightColor.redComp;
                    resultColor.greenComp += nDotLPt * col.y * pointLights[ptLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLPt * col.z * pointLights[ptLight].lightColor.blueComp;

                    //log << "resultColor.redComp after point light: " << resultColor.redComp << endl;
                    //log << "resultColor.greenComp after point light: " << resultColor.greenComp << endl;
                    //log << "resultColor.blueComp after point light: " << resultColor.blueComp << endl;
                }

                //if you want the lightmap to LEAVE OUT texture information (only lighting)
                else{
                    //adding n dot L to the color components
                    resultColor.redComp += nDotLPt * pointLights[ptLight].lightColor.redComp;
                    resultColor.greenComp += nDotLPt * pointLights[ptLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLPt * pointLights[ptLight].lightColor.blueComp;
                }

            }

        }





        MyRay dirShadowRay;
        //now deal with shadowing with directional lights
        for(int dirLight = 0; dirLight < directionalLights.size(); dirLight++){

            //doing a deep copy
            //now my shadow ray is a little more up along the hit triangle surface's normal
            dirShadowRay.Equals( GetDirShadowRay(modifiedIntPt, directionalLights[dirLight]) );

            //if we're NOT in shadow from the directional light
            if( !(IsInShadowDir(dirShadowRay)) ){

                Eigen::Vector4f LDir;

                //don't have to normalize cause directional shadow ray is already normalized
                LDir.head<3>() = dirShadowRay.direction;
                LDir[3] = 0.0f;


                float nDotLDir = hitNormal.dot(LDir);


                if(nDotLDir < 0.0f){
                    nDotLDir = 0.0f;
                }

                //if you want the lightmap to contain texture information
                if(curObj.hasText){
                    //indexing into the texture
                    float xText = invUV[0] * (float)curObj.texture->width;
                    float yText = invUV[1] * (float)curObj.texture->height;

                    se::math::Vector3D textCoord(xText, yText, 0.0f);

                    se::graphics::image::Color4f col = curObj.texture->read(textCoord, FloatImageRGBA::LINEAR);

                    //log << "u: " << u << endl;
                    //log << "v: " << v << endl;

                    //adding n dot L to the color components
                    resultColor.redComp += nDotLDir * col.x * directionalLights[dirLight].lightColor.redComp;
                    resultColor.greenComp += nDotLDir * col.y * directionalLights[dirLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLDir * col.z * directionalLights[dirLight].lightColor.blueComp;

                    //log << "resultColor.redComp after direction light: " << resultColor.redComp << endl;
                    //log << "resultColor.greenComp after direction light: " << resultColor.greenComp << endl;
                    //log << "resultColor.blueComp after direction light: " << resultColor.blueComp << endl;
                }

                //if you want the lightmap to LEAVE OUT texture information (only lighting)
                else{
                    //adding n dot L to the color components
                    resultColor.redComp += nDotLDir * directionalLights[dirLight].lightColor.redComp;
                    resultColor.greenComp += nDotLDir * directionalLights[dirLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLDir * directionalLights[dirLight].lightColor.blueComp;
                }

            }
        }

        //cap all the color component to one in case they go over
        if(resultColor.redComp > 1.0f){
            resultColor.redComp = 1.0f;
        }
        if(resultColor.greenComp > 1.0f){
            resultColor.greenComp = 1.0f;
        }
        if(resultColor.blueComp > 1.0f){
            resultColor.blueComp = 1.0f;
        }

        //log << "final resultColor red comp.: " << resultColor.redComp << endl;
        //log << "final resultColor green comp.: " << resultColor.greenComp << endl;
        //log << "final resultColor blue comp.: " << resultColor.blueComp << endl;

        return resultColor;
    }

    else
    {
        //if we don't have an intersection, return black
        return resultColor;
    }

}

//this function will return a hit UV for the triangle we hit
bool Lightmap::IsUVHit(float v, float u, Triangle tri)
{
    float betaTemp = -10.0f;
    float gammaTemp = -10.0f;
    float alphaTemp = -10.0f;

    //this 2-by-2 matrix has the point subtractions
    Eigen::Matrix<float, 2, 2> mat1;

    //this 2-by-1 matrix has the R0-P2 matrix
    Eigen::Matrix<float, 2, 1> mat2;

    //this 2-by-1 matrix stores the answer
    Eigen::Matrix<float, 2, 1> ans;


    mat1 << (tri.vertices[1].textCoord[0] - tri.vertices[2].textCoord[0]), (tri.vertices[0].textCoord[0] - tri.vertices[2].textCoord[0]),
            (tri.vertices[1].textCoord[1] - tri.vertices[2].textCoord[1]), (tri.vertices[0].textCoord[1] - tri.vertices[2].textCoord[1]);



    mat2 << (u - tri.vertices[2].textCoord[0]),
            (v - tri.vertices[2].textCoord[1]);

    ans = mat1.inverse() * mat2;

    betaTemp = ans(0,0);
    gammaTemp = ans(1,0);

    //log << "beta: " << beta << endl;
    //log << "gamma: " << gamma << endl;
    //log << "t: " << ans(2,0) << endl;

    if(betaTemp >= 0 && betaTemp <= 1 && gammaTemp >= 0 && gammaTemp <= 1)
    {
        alphaTemp = 1 - betaTemp - gammaTemp;

        if(alphaTemp >= 0 && alphaTemp <= 1)
        {

            baryTemp.alphaTemp = alphaTemp;
            baryTemp.betaTemp = betaTemp;
            baryTemp.gammaTemp = gammaTemp;

            //return true cause we hit something
            return true;

        }
        else
        {
            //we don't hit so return false
            return false;
        }

    }

    else
    {
        //we don't hit so return false
        return false;
    }
}





/////////////////////////////////////////////////
/////////Directional Light Shadow Functions//////
/////////////////////////////////////////////////

MyRay Lightmap::GetDirShadowRay(Eigen::Vector3f intersection_pt, DirLight dirLight){
    MyRay dirShadowRay;

    //this shadow ray's origin is the intersection point we got from ray tracing
    dirShadowRay.origin = intersection_pt;

    //the direction of this shadow ray is the direction of the directional light
    dirShadowRay.direction = dirLight.dirLightL.head<3>();

    //normalize the directional lights shadow ray cause we're gonna check if the t is greater than 0
    dirShadowRay.direction.normalize();

    return dirShadowRay;

}


bool Lightmap::IsInShadowDir(MyRay dirShadowRay){
    float tDirShadow = FLT_MAX;

    //iterating through all objects on the list
    for(int j = 0; j < objectList.size(); j++){

        std::vector<Triangle> & triList = objectList[j].GetTriangles(dirShadowRay);

        //iterating through all the triangles in the trianglelist of the current object
        for(int i = 0; i < triList.size(); i++)
        {
            Eigen::Vector3f norToTri;

            //finding the normal to the currently looked-at triangle
            norToTri = GetNormalToTri(triList[i]);

            //for triangles that are parallel to the ray...they'll never intersect
            if( fabs(norToTri.dot(dirShadowRay.direction)) <= 1e-8)
            {
                continue;
            }

            //get the t of intersection
            tDirShadow = IsGenericHit(triList[i], dirShadowRay);


            //if the t of the shadow is valid and is greater than 0...
            if(tDirShadow > 0.0f && tDirShadow != FLT_MAX)
            {
                //log << "tDirShadow: " << tDirShadow << endl;
                //...return true cause the point's in shadow
                return true;
            }

        }
    }


    //if we got this far then we're not in shadow
    return false;
}


/////////////////////////////////////////////////
/////////Directional Light Shadow Functions//////
/////////////////////////////////////////////////






/////////////////////////////////////////////
/////////Point Light Shadow Functions////////
/////////////////////////////////////////////

MyRay Lightmap::GetPtShadowRay(Eigen::Vector3f intersection_pt, PointLight ptLight){
    MyRay ptShadowRay;

    //this shadow ray's origin is the intersection point we got from ray tracing
    ptShadowRay.origin = intersection_pt;


    //the direction of this pt. shadow ray is from the intersection point to the light position
    //i'm NOT normalizing this direction cause i'm gonna check if t is between 0 and 1
    ptShadowRay.direction = ptLight.lightPos - intersection_pt;


    return ptShadowRay;
}

//function to test if we hit a point light source
bool Lightmap::IsInShadowPt(MyRay ptShadowRay){

    float tPtShadow = 0.0f;

    //precalculate the ptShadowRay's direction to a temp variable to test for parallelism
    Eigen::Vector3f tempShadowDir = ptShadowRay.direction;
    tempShadowDir.normalize();

    //iterating through all objects on the list
    for(int j = 0; j < objectList.size(); j++){

        std::vector<Triangle> & triList = objectList[j].GetTriangles(ptShadowRay);

        //iterating through all the triangles in the trianglelist of the current object
        for(int i = 0; i < triList.size(); i++)
        {

            Eigen::Vector3f norToTri;

            //finding the normal to the currently looked-at triangle
            norToTri = GetNormalToTri(triList[i]);


            //for triangles that are parallel to the ray...they'll never intersect
            if( fabs(norToTri.dot(tempShadowDir)) <= 1e-8)
            {
                continue;
            }

            //get the t of intersection
            tPtShadow = IsGenericHit(triList[i], ptShadowRay);


            //if the t of the shadow is valid and is between 0 and 1...
            if(tPtShadow > 0.0f && tPtShadow < 1.0f)
            {
                //log << "tPtShadow: " << tPtShadow << endl;

                //...return true cause the point's in shadow
                return true;
            }

        }

    }


    //if we got this far then we're not in shadow
    return false;
}

/////////////////////////////////////////////
/////////Point Light Shadow Functions////////
/////////////////////////////////////////////




//this IsHit function doesn't fuck around with baryTemp
//it's used for the shadow functions
float Lightmap::IsGenericHit(Triangle tri, MyRay testRay)
{
    float betaTemp = -10.0f;
    float gammaTemp = -10.0f;
    float alphaTemp = -10.0f;

    //this 3-by-3 matrix has the point subtractions
    Eigen::Matrix3f mat1;

    //this 3-by-1 matrix has the R0-P2 matrix
    Eigen::Matrix<float, 3, 1> mat2;

    //this 3-by-1 matrix stores the answer
    Eigen::Matrix<float, 3, 1> ans;


    mat1 << (tri.vertices[1].position[0] - tri.vertices[2].position[0]), (tri.vertices[0].position[0] - tri.vertices[2].position[0]), (-testRay.direction[0]),
            (tri.vertices[1].position[1] - tri.vertices[2].position[1]), (tri.vertices[0].position[1] - tri.vertices[2].position[1]), (-testRay.direction[1]),
            (tri.vertices[1].position[2] - tri.vertices[2].position[2]), (tri.vertices[0].position[2] - tri.vertices[2].position[2]), (-testRay.direction[2]);



    mat2 << (testRay.origin[0] - tri.vertices[2].position[0]),
            (testRay.origin[1] - tri.vertices[2].position[1]),
            (testRay.origin[2] - tri.vertices[2].position[2]);

    ans = mat1.inverse() * mat2;

    betaTemp = ans(0,0);
    gammaTemp = ans(1,0);

    //log << "beta: " << beta << endl;
    //log << "gamma: " << gamma << endl;
    //log << "t: " << ans(2,0) << endl;

    if(betaTemp >= 0 && betaTemp <= 1 && gammaTemp >= 0 && gammaTemp <= 1)
    {
        alphaTemp = 1 - betaTemp - gammaTemp;

        if(alphaTemp >= 0 && alphaTemp <= 1)
        {

            return ans(2,0);

        }
        else
        {
            //we don't hit so return invalid shit
            return -100.0f;
        }

    }

    else
    {
        //we don't hit so return invalid shit
        return -100.0f;
    }
}


//return the UV
void Lightmap::GetHitUV(Triangle hitTri)
{
    //log << "alpha in GetHitUV: " << alpha << endl;
    //log << "beta in GetHitUV: " << beta << endl;
    //log << "gamma in GetHitUV: " << gamma << endl;

    for(int i = 0; i < 2; i++)
    {
        invUV[i] = (hitTri.vertices[2].textCoord[i] * alpha) + (hitTri.vertices[1].textCoord[i] * beta) + (hitTri.vertices[0].textCoord[i] * gamma);
    }


    //log << "u: " << invUV[0] << endl;
    //log << "v: " << invUV[1] << endl;

}



//getting the normalized normal of the intersection pt of the triangle
Eigen::Vector4f Lightmap::GetHitNormal(Triangle hitTri)
{
    Eigen::Vector4f hitNormal;

    for(int i = 0; i < 3; i++)
    {
        //setting the normal's x, y and z
        hitNormal[i] = (hitTri.vertices[2].normal[i] * alpha) + (hitTri.vertices[1].normal[i] * beta) + (hitTri.vertices[0].normal[i] * gamma);
    }
    hitNormal[3] = 0.0f;

    //normalizing the normal :)
    hitNormal.normalize();


    return hitNormal;
}


//return the intersection point using alpha, beta, gamma
Eigen::Vector3f Lightmap::GetPoint(Triangle hitTri)
{
    //log << "alpha in GetPoint: " << alpha << endl;
    //log << "beta in GetPoint: " << beta << endl;
    //log << "gamma in GetPoint: " << gamma << endl;

    Eigen::Vector3f realPt;

    for(int i = 0; i < 3; i++)
    {
        realPt[i] = (hitTri.vertices[2].position[i] * alpha) + (hitTri.vertices[1].position[i]  * beta) + (hitTri.vertices[0].position[i] * gamma);
    }

    return realPt;

}


//getting the normal to the triangle i'm looking at, used for parallel testing
Eigen::Vector3f Lightmap::GetNormalToTri(Triangle tri)
{
    Eigen::Vector3f vect1;
    Eigen::Vector3f vect2;

    Eigen::Vector3f planeNormal;

    //this is creating vec P2P0
    vect1[0] = tri.vertices[0].position[0] - tri.vertices[2].position[0];
    vect1[1] = tri.vertices[0].position[1] - tri.vertices[2].position[1];
    vect1[2] = tri.vertices[0].position[2] - tri.vertices[2].position[2];
    //this is creating vec P2P0


    //this is creating vec P2P1
    vect2[0] = tri.vertices[1].position[0] - tri.vertices[2].position[0];
    vect2[1] = tri.vertices[1].position[1] - tri.vertices[2].position[1];
    vect2[2] = tri.vertices[1].position[2] - tri.vertices[2].position[2];
    //this is creating vec P2P1


    //getting the normal of the plane and normalizing it
    //this gives is A, B, and C of the plane equation
    planeNormal = vect1.cross(vect2);

    planeNormal.normalize();

    return planeNormal;


}

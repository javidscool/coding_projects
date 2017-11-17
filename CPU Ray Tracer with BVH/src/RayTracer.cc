#include "RayTracer.h"
#include<se/graphics/image/formats/RgbeImage.h>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace se::graphics::image;

//ray-tracer constructor
RayTracer::RayTracer(std::vector<Object> objList, std::vector<BVH> bvhList, const int width, const int height)
{
    //set the object list
    objectList = objList;


    //set the BVH list
    BVHList = bvhList;

    //initializing (u,v) to bunk values
    invUV[0] = -1.0f;
    invUV[1] = -1.0f;


    window_width = width;
    window_height = height;

    near_plane = 0.01f;
    float fov = 3.14159 / 4;
    float aspect = window_width/window_height;

    float half_height = near_plane * tan(fov/2);
    float half_width = aspect * half_height;

    height_of_pixel = 2 * half_height / window_height;
    width_of_pixel = 2 * half_width / window_width;

    //this gives me the position of the middle of the top left pixel
    uL_x = -half_width + (0.5 * width_of_pixel);
    uL_y = half_height - (0.5 * height_of_pixel);



    //adding some directional lights (these coordinates are pointing twords the directional light)
    //directionalLights.push_back(  DirLight(Eigen::Vector4f(3.0f, -4.0f, 1.0f, 0.0f), Color(1.0f, 0.0f, 0.0f)) );
    directionalLights.push_back(  DirLight(Eigen::Vector4f(0.0f, 0.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)) );

    //adding some point lights (these coordinates are the POSITIONS of the point lights)
    //this point light is to the left of the robot
    //pointLights.push_back( PointLight(Eigen::Vector3f(-2.0f, 0.0f, -9.0f), Color(0.0f, 1.0f, 0.0f)) );
    //pointLights.push_back( PointLight(Eigen::Vector3f(-3.0f, -4.0f, 1.0f), Color(0.0f, 0.0f, 1.0f)) );

    log << endl;
    log << "Number of directional lights: " << directionalLights.size() << endl;
    log << "Number of point lights: " << pointLights.size() << endl;

    log << "alphaTemp: " << baryTemp.alphaTemp << endl;
    log << "betaTemp: " << baryTemp.betaTemp << endl;
    log << "gammaTemp: " << baryTemp.gammaTemp << endl;

    //setting up the background
    ifstream bgf("bg.hdr", ifstream::in | ifstream::binary);
    formats::RgbeImage bgim(bgf);
    background.reset(new st::image::pano::LatitudeLongitude(bgim));

    fout.open("intersection_list.txt");

}




///////////////////////////////////
////Adaptive Supersampling code////
///////////////////////////////////

//this is the main supersampling function that shoots the five origin rays
Color RayTracer::AdaptiveSuperSamp(MyRay originRay, int j, int i){
    //initializing to the maximum float
    float tMin = FLT_MAX;

    float tTemp = 0.0f;
    Triangle hitTri;

    Color resultColor;

    //first create my other 4 rays
    std::vector<MyRay> cornerRays = GetAdapRays(originRay, j, i);

    //now run the ray tracer and get back the colors for all 5 of these rays
    Color middleCol = GetColor(originRay);

    Color upLCol = GetColor(cornerRays[0]);

    Color downLCol = GetColor(cornerRays[1]);

    Color upRCol = GetColor(cornerRays[2]);

    Color downRCol = GetColor(cornerRays[3]);


    //now compare the colors
    if(IsColorTooDiff(middleCol, upLCol)){
        resultColor += AdaptiveSuperPass2(originRay, cornerRays[0], middleCol, upLCol, 0);
    }
    else{
        resultColor += (middleCol + upLCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, downLCol)){
        resultColor += AdaptiveSuperPass2(originRay, cornerRays[1], middleCol, downLCol, 1);
    }
    else{
        resultColor += (middleCol + downLCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, upRCol)){
        resultColor += AdaptiveSuperPass2(originRay, cornerRays[2], middleCol, upRCol, 2);
    }
    else{
        resultColor += (middleCol + upRCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, downRCol)){
        resultColor += AdaptiveSuperPass2(originRay, cornerRays[3], middleCol, downRCol, 3);
    }
    else{
        resultColor += (middleCol + downRCol) / 2.0f;
    }


    resultColor *= (0.25f);

    return resultColor;

}

//this function gets our four corner rays
std::vector<MyRay> RayTracer::GetAdapRays(MyRay middleRay, int j, int i){
    std::vector<MyRay> cornerRays;


    //directional vector used in GetOriginRay that's used to get the other 4 rays directions
    Eigen::Vector3f direct;

    //the x-coord of direction
    direct[0] = i * (width_of_pixel) + uL_x;

    //the y-coord of direction
    direct[1] = -j * (height_of_pixel) + uL_y;

    //the z-coord of direction
    direct[2] = -near_plane;



    //creating the upper left ray
    Eigen::Vector3f upLOrigin = middleRay.origin;

    Eigen::Vector3f upLDirection;

    upLDirection[0] = direct[0] - (0.5 * width_of_pixel);
    upLDirection[1] = direct[1] + (0.5 * height_of_pixel);
    upLDirection[2] = direct[2];

    //copying direction vector so I can normalize it and pass it on to the ray constructor
    Eigen::Vector3f normalUpLDirect = upLDirection;
    normalUpLDirect.normalize();

    //creating the upL ray with a depth of zero
    MyRay upL(upLOrigin, upLDirection, normalUpLDirect, 0, 0);



    //creating the lower left ray
    Eigen::Vector3f downLOrigin = middleRay.origin;

    Eigen::Vector3f downLDirection;

    downLDirection[0] = direct[0] - (0.5 * width_of_pixel);
    downLDirection[1] = direct[1] - (0.5 * height_of_pixel);
    downLDirection[2] = direct[2];

    //copying direction vector so I can normalize it and pass it on to the ray constructor
    Eigen::Vector3f normalDownLDirect = downLDirection;
    normalDownLDirect.normalize();

    //creating the downL ray with a depth of zero
    MyRay downL(downLOrigin, downLDirection, normalDownLDirect, 0, 0);



    //creating the upper right ray
    Eigen::Vector3f upROrigin = middleRay.origin;

    Eigen::Vector3f upRDirection;

    upRDirection[0] = direct[0] + (0.5 * width_of_pixel);
    upRDirection[1] = direct[1] + (0.5 * height_of_pixel);
    upRDirection[2] = direct[2];

    //copying direction vector so I can normalize it and pass it on to the ray constructor
    Eigen::Vector3f normalUpRDirect = upRDirection;
    normalUpRDirect.normalize();

    //creating the upR ray with a depth of zero
    MyRay upR(upROrigin, upRDirection, normalUpRDirect, 0, 0);



    //creating the lower right ray
    Eigen::Vector3f downROrigin = middleRay.origin;

    Eigen::Vector3f downRDirection;

    downRDirection[0] = direct[0] + (0.5 * width_of_pixel);
    downRDirection[1] = direct[1] - (0.5 * height_of_pixel);
    downRDirection[2] = direct[2];

    //copying direction vector so I can normalize it and pass it on to the ray constructor
    Eigen::Vector3f normalDownRDirect = downRDirection;
    normalDownRDirect.normalize();

    //creating the downR ray with a depth of zero
    MyRay downR(downROrigin, downRDirection, normalDownRDirect, 0, 0);



    //adding all these rays to our cornerRays list
    cornerRays.push_back(upL);
    cornerRays.push_back(downL);
    cornerRays.push_back(upR);
    cornerRays.push_back(downR);


    return cornerRays;
}

//return true if the colors from two rays are too different, false otherwise
bool RayTracer::IsColorTooDiff(Color color1, Color color2){
    //the colors cannot be more different than this
    float allowableDiff = 0.3f;

    //get the difference in colors
    float redDiff = color1.redComp - color2.redComp;
    float greenDiff = color1.greenComp - color2.greenComp;
    float blueDiff = color1.blueComp - color2.blueComp;

    //calculate the total color difference
    float colorDiff = fabs(redDiff) + fabs(greenDiff) + fabs(blueDiff);

    if(colorDiff > allowableDiff){
        return true;
    }
    else{
        return false;
    }

}

//this is the recursive adaptive shit that gets run if two rays return different colors
Color RayTracer::AdaptiveSuperPass2(MyRay ray1, MyRay ray2, Color ray1Col, Color ray2Col, int situation){
    //ray1 is middle ray
    //ray2 changes depending on the situation


    Color resultColor;

    ray1.superSampleDepth++;
    ray2.superSampleDepth++;

    int superSampMin = min(ray1.superSampleDepth, ray2.superSampleDepth);

    //i'm recursing too much (sampling too deep)...so return
    if(superSampMin > 2){
        return ( (ray1Col + ray2Col) / 2.0f);
    }

    //first create 3 rays
    std::vector<MyRay> remainderRays = GetAdapRaysPass2(ray1, ray2, situation, superSampMin);

    //making a vector of ordered rays (how I like 'em)
    std::vector<MyRay> rays;


    Color middleCol;
    Color upLCol;
    Color downLCol;
    Color upRCol;
    Color downRCol;

    //ordering rays like dis: middle, upL, downL, upR, downR
    if(situation == 0){
        rays.push_back(remainderRays[2]);
        rays.push_back(ray2);
        rays.push_back(remainderRays[0]);
        rays.push_back(remainderRays[1]);
        rays.push_back(ray1);

        //now run the ray tracer and get back the colors for all 5 of these rays
        middleCol = GetColor(rays[0]);

        upLCol = ray2Col;

        downLCol = GetColor(rays[2]);

        upRCol = GetColor(rays[3]);

        downRCol = ray1Col;
    }


    else if(situation == 1){
        rays.push_back(remainderRays[2]);
        rays.push_back(remainderRays[0]);
        rays.push_back(ray2);
        rays.push_back(ray1);
        rays.push_back(remainderRays[1]);

        //now run the ray tracer and get back the colors for all 5 of these rays
        middleCol = GetColor(rays[0]);

        upLCol = GetColor(rays[1]);

        downLCol = ray2Col;

        upRCol = ray1Col;

        downRCol = GetColor(rays[4]);
    }


    else if(situation == 2){
        rays.push_back(remainderRays[2]);
        rays.push_back(remainderRays[0]);
        rays.push_back(ray1);
        rays.push_back(ray2);
        rays.push_back(remainderRays[1]);

        //now run the ray tracer and get back the colors for all 5 of these rays
        middleCol = GetColor(rays[0]);

        upLCol = GetColor(rays[1]);

        downLCol = ray1Col;

        upRCol = ray2Col;

        downRCol = GetColor(rays[4]);
    }


    else if(situation == 3){
        rays.push_back(remainderRays[2]);
        rays.push_back(ray1);
        rays.push_back(remainderRays[0]);
        rays.push_back(remainderRays[1]);
        rays.push_back(ray2);

        //now run the ray tracer and get back the colors for all 5 of these rays
        middleCol = GetColor(rays[0]);

        upLCol = ray1Col;

        downLCol = GetColor(rays[2]);

        upRCol = GetColor(rays[3]);

        downRCol = ray2Col;
    }



    //now compare the colors
    if(IsColorTooDiff(middleCol, upLCol)){
        resultColor += AdaptiveSuperPass2(rays[0], rays[1], middleCol, upLCol, 0);
    }
    else{
        resultColor += (middleCol + upLCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, downLCol)){
        resultColor += AdaptiveSuperPass2(rays[0], rays[2], middleCol, downLCol, 1);
    }
    else{
        resultColor += (middleCol + downLCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, upRCol)){
        resultColor += AdaptiveSuperPass2(rays[0], rays[3], middleCol, upRCol, 2);
    }
    else{
        resultColor += (middleCol + upRCol) / 2.0f;
    }

    if(IsColorTooDiff(middleCol, downRCol)){
        resultColor += AdaptiveSuperPass2(rays[0], rays[4], middleCol, downRCol, 3);
    }
    else{
        resultColor += (middleCol + downRCol) / 2.0f;
    }


    resultColor *= (0.25f);

    return resultColor;
}

//creating the three rays needed for the deeper adaptive supersampling if we go deeper than the five origin rays
//i'm creating three rays because we alredy have the other two (ray1, and ray2)... so I don't need to make them
std::vector<MyRay> RayTracer::GetAdapRaysPass2(MyRay ray1, MyRay ray2, int situation, int superSampMin){
    //ray1 is middle ray
    //ray2 changes depending on the situation

    std::vector<MyRay> remainderRays;

    //situation 0 is when the upL and middle rays are different
    if(situation == 0){
        //creating the lower left ray
        Eigen::Vector3f downLOrigin = ray1.origin;

        Eigen::Vector3f downLDirection;

        downLDirection[0] = ray2.direction[0];
        downLDirection[1] = ray1.direction[1];
        downLDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalDownLDirect = downLDirection;
        normalDownLDirect.normalize();

        //creating the downL ray with a depth of zero
        MyRay downL(downLOrigin, downLDirection, normalDownLDirect, 0, superSampMin);



        //creating upper right ray
        Eigen::Vector3f upROrigin = ray1.origin;

        Eigen::Vector3f upRDirection;

        upRDirection[0] = ray1.direction[0];
        upRDirection[1] = ray2.direction[1];
        upRDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalUpRDirect = upRDirection;
        normalUpRDirect.normalize();

        //creating the upR ray with a depth of zero
        MyRay upR(upROrigin, upRDirection, normalUpRDirect, 0, superSampMin);



        //create the new middle
        Eigen::Vector3f newMidOrigin = ray1.origin;

        Eigen::Vector3f newMidDirection;

        newMidDirection[0] = (ray1.direction[0] + ray2.direction[0]) / 2;
        newMidDirection[1] = (ray1.direction[1] + ray2.direction[1]) / 2;
        newMidDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalNewMidDirect = newMidDirection;
        normalNewMidDirect.normalize();

        //creating the new middle ray with a depth of zero
        MyRay newMid(newMidOrigin, newMidDirection, normalNewMidDirect, 0, superSampMin);

        //adding these rays to our remainder rays list
        remainderRays.push_back(downL);
        remainderRays.push_back(upR);
        remainderRays.push_back(newMid);
    }



    //situation 1 is when the downL and middle rays are different
    else if(situation == 1){
        //creating the upper left ray
        Eigen::Vector3f upLOrigin = ray1.origin;

        Eigen::Vector3f upLDirection;

        upLDirection[0] = ray2.direction[0];
        upLDirection[1] = ray1.direction[1];
        upLDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalUpLDirect = upLDirection;
        normalUpLDirect.normalize();

        //creating the downL ray with a depth of zero
        MyRay upL(upLOrigin, upLDirection, normalUpLDirect, 0, superSampMin);



        //creating lower right ray
        Eigen::Vector3f downROrigin = ray1.origin;

        Eigen::Vector3f downRDirection;

        downRDirection[0] = ray1.direction[0];
        downRDirection[1] = ray2.direction[1];
        downRDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalDownRDirect = downRDirection;
        normalDownRDirect.normalize();

        //creating the upR ray with a depth of zero
        MyRay downR(downROrigin, downRDirection, normalDownRDirect, 0, superSampMin);



        //create the new middle
        Eigen::Vector3f newMidOrigin = ray1.origin;

        Eigen::Vector3f newMidDirection;

        newMidDirection[0] = (ray1.direction[0] + ray2.direction[0]) / 2;
        newMidDirection[1] = (ray1.direction[1] + ray2.direction[1]) / 2;
        newMidDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalNewMidDirect = newMidDirection;
        normalNewMidDirect.normalize();

        //creating the new middle ray with a depth of zero
        MyRay newMid(newMidOrigin, newMidDirection, normalNewMidDirect, 0, superSampMin);

        //adding these rays to our remainder rays list
        remainderRays.push_back(upL);
        remainderRays.push_back(downR);
        remainderRays.push_back(newMid);
    }



    //situation 2 is when the upR and middle rays are different
    else if(situation == 2){
        //creating the upper left ray
        Eigen::Vector3f upLOrigin = ray1.origin;

        Eigen::Vector3f upLDirection;

        upLDirection[0] = ray1.direction[0];
        upLDirection[1] = ray2.direction[1];
        upLDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalUpLDirect = upLDirection;
        normalUpLDirect.normalize();

        //creating the downL ray with a depth of zero
        MyRay upL(upLOrigin, upLDirection, normalUpLDirect, 0, superSampMin);



        //creating lower right ray
        Eigen::Vector3f downROrigin = ray1.origin;

        Eigen::Vector3f downRDirection;

        downRDirection[0] = ray2.direction[0];
        downRDirection[1] = ray1.direction[1];
        downRDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalDownRDirect = downRDirection;
        normalDownRDirect.normalize();

        //creating the upR ray with a depth of zero
        MyRay downR(downROrigin, downRDirection, normalDownRDirect, 0, superSampMin);



        //create the new middle
        Eigen::Vector3f newMidOrigin = ray1.origin;

        Eigen::Vector3f newMidDirection;

        newMidDirection[0] = (ray1.direction[0] + ray2.direction[0]) / 2;
        newMidDirection[1] = (ray1.direction[1] + ray2.direction[1]) / 2;
        newMidDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalNewMidDirect = newMidDirection;
        normalNewMidDirect.normalize();

        //creating the new middle ray with a depth of zero
        MyRay newMid(newMidOrigin, newMidDirection, normalNewMidDirect, 0, superSampMin);

        //adding these rays to our remainder rays list
        remainderRays.push_back(upL);
        remainderRays.push_back(downR);
        remainderRays.push_back(newMid);
    }



    //situation 3 is when the downR and middle rays are different
    else if(situation == 3){
        //creating the lower left ray
        Eigen::Vector3f downLOrigin = ray1.origin;

        Eigen::Vector3f downLDirection;

        downLDirection[0] = ray1.direction[0];
        downLDirection[1] = ray2.direction[1];
        downLDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalDownLDirect = downLDirection;
        normalDownLDirect.normalize();

        //creating the downL ray with a depth of zero
        MyRay downL(downLOrigin, downLDirection, normalDownLDirect, 0, superSampMin);



        //creating upper right ray
        Eigen::Vector3f upROrigin = ray1.origin;

        Eigen::Vector3f upRDirection;

        upRDirection[0] = ray2.direction[0];
        upRDirection[1] = ray1.direction[1];
        upRDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalUpRDirect = upRDirection;
        normalUpRDirect.normalize();

        //creating the upR ray with a depth of zero
        MyRay upR(upROrigin, upRDirection, normalUpRDirect, 0, superSampMin);



        //create the new middle
        Eigen::Vector3f newMidOrigin = ray1.origin;

        Eigen::Vector3f newMidDirection;

        newMidDirection[0] = (ray1.direction[0] + ray2.direction[0]) / 2;
        newMidDirection[1] = (ray1.direction[1] + ray2.direction[1]) / 2;
        newMidDirection[2] = ray1.direction[2];

        //copying direction vector so I can normalize it and pass it on to the ray constructor
        Eigen::Vector3f normalNewMidDirect = newMidDirection;
        normalNewMidDirect.normalize();

        //creating the new middle ray with a depth of zero
        MyRay newMid(newMidOrigin, newMidDirection, normalNewMidDirect, 0, superSampMin);

        //adding these rays to our remainder rays list
        remainderRays.push_back(downL);
        remainderRays.push_back(upR);
        remainderRays.push_back(newMid);
    }


    return remainderRays;
}

///////////////////////////////////
////Adaptive Supersampling code////
///////////////////////////////////


//this is the ray bounding box intersection to check if the ray intersects the bounding box of the node of the BVH
bool RayTracer::IntersectBVHNode(const AABBox &bounds, Eigen::Vector3f invDir, Eigen::Vector3i dirIsNeg, Eigen::Vector3f rayOrig, float rayMaxt){
    float tMin, tMax, tyMin, tyMax, tzMin, tzMax;

    ///check for intersection with the x slab///
	//if the x direction of the ray is negative
	if(dirIsNeg[0] == 1){
		tMin = (bounds.pMax[0] - rayOrig[0]) * invDir[0];
		tMax = (bounds.pMin[0] - rayOrig[0]) * invDir[0];
	}

	//if the x direction is positive
	else{
		tMin = (bounds.pMin[0] - rayOrig[0]) * invDir[0];
		tMax = (bounds.pMax[0] - rayOrig[0]) * invDir[0];
	}
	///check for intersection with the x slab///


	///check for intersection with the y slab///
	//if the y direction of the ray is negative
	if(dirIsNeg[1] == 1){
		tyMin = (bounds.pMax[1] - rayOrig[1]) * invDir[1];
		tyMax = (bounds.pMin[1] - rayOrig[1]) * invDir[1];
	}

	//if the y direction is positive
	else{
		tyMin = (bounds.pMin[1] - rayOrig[1]) * invDir[1];
		tyMax = (bounds.pMax[1] - rayOrig[1]) * invDir[1];
	}
	///check for intersection with the y slab///


	if ((tMin > tyMax) || (tyMin > tMax)){
		return false;
	}

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;


	///check for intersection with the z slab///
	//if the z direction of the ray is negative
	if(dirIsNeg[2] == 1){
		tzMin = (bounds.pMax[2] - rayOrig[2]) * invDir[2];
		tzMax = (bounds.pMin[2] - rayOrig[2]) * invDir[2];
	}

	//if the z direction is positive
	else{
		tzMin = (bounds.pMin[2] - rayOrig[2]) * invDir[2];
		tzMax = (bounds.pMax[2] - rayOrig[2]) * invDir[2];
	}
	///check for intersection with the z slab///

	if ((tMin > tzMax) || (tzMin > tMax)){
		return false;
	}

    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;


	return (tMin < rayMaxt) && (tMax > 0.0f);
}


bool RayTracer::IsInBox(const AABBox &bounds, Eigen::Vector3f pt){
    return (pt[0] >= bounds.pMin[0] && pt[0] <= bounds.pMax[0] &&
            pt[1] >= bounds.pMin[1] && pt[1] <= bounds.pMax[1] &&
            pt[2] >= bounds.pMin[2] && pt[2] <= bounds.pMax[2]);
}



//MAIN FUNCTION THAT DOES EVERYTHING
Color RayTracer::GetColor(MyRay ray)
{
    //initializing to the maximum float
    float tMin = FLT_MAX;

    float tTemp = 0.0f;
    Triangle hitTri;

    Color resultColor;

    Object hitObject;

    Eigen::Vector3f invDir = Eigen::Vector3f( (1.0f / ray.normalizedDirect[0]), (1.0f / ray.normalizedDirect[1]), (1.0f / ray.normalizedDirect[2]) );
    Eigen::Vector3i dirIsNeg = Eigen::Vector3i( (invDir[0] < 0), (invDir[1] < 0), (invDir[2] < 0) );

    int todoOffset;
	int nodeNum;
	int toDo[64];

	int numPrimitives = 0;
    bool isInBox = false;


    //increase the ray depth
    ray.depth++;

    //if the ray has gone deep enough, return zero color through the recursion
    if(ray.depth == 8){
        return resultColor;
    }

    //iterating through all objects on the list
    for(int j = 0; j < BVHList.size(); j++){
        todoOffset = 0;
        nodeNum = 0;
        for(int k = 0; k < 64; k++){
            toDo[k] = 0;
        }

        std::vector<Triangle> & triList = BVHList[j].triList;

        while(true){

            const LinearBVHNode *node = &(BVHList[j].flatList[nodeNum]);

            numPrimitives = node->numPrimitives;

            //if i'm in the box...
            isInBox = IsInBox(node->bounds, ray.origin);

            if( isInBox && numPrimitives == 0){
                //...always visit left child then right child
                toDo[todoOffset++] = node->secondChildOffset;
                nodeNum = nodeNum + 1;

                continue;
            }

            //checking to see if we get an intersection with the currently looked at bounding box
            if(IntersectBVHNode(node->bounds, invDir, dirIsNeg, ray.origin, tMin) || isInBox){

                if(node->numPrimitives > 0){

                    //we hit a leaf node, so intersect with the primitives in the leaf node
                    for(int i = 0; i < node->numPrimitives; i++)
                    {
                        Eigen::Vector3f normalToTri;

                        //finding the normal to the currently looked-at triangle
                        normalToTri = GetNormalToTri(triList[node->primitivesOffset + i]);

                        //for triangles that are parallel to the ray...they'll never intersect
                        if( fabs(normalToTri.dot(ray.normalizedDirect)) <= 1e-8)
                        {
                            continue;
                        }

                        //get a potential alpha-beta-gamma
                        tTemp = IsHit(triList[node->primitivesOffset + i], ray);

                        //if the t is valid and less than the currently stored t...
                        if(tTemp > 0.0f && tTemp < tMin)
                        {
                            //..get the new t
                            tMin = tTemp;
                            //log << "tMin: " << tMin << endl;

                            alpha = baryTemp.alphaTemp;
                            beta = baryTemp.betaTemp;
                            gamma = baryTemp.gammaTemp;

                            //get the triangle that was hit
                            hitTri.Equals(triList[node->primitivesOffset + i]);
                            hitObject = objectList[j];
                        }

                    }

                    //getting next node to test (if there is a next node to test)
                    if(todoOffset == 0){
                        break;
                    }
                    nodeNum = toDo[--todoOffset];

                }

                //else, we need to traverse the BVH tree more
                else{

                    //if our split axis direction was negative, visit the second child before the first child
                    if(dirIsNeg[node->axis]){
                        toDo[todoOffset++] = nodeNum + 1;
                        nodeNum = node->secondChildOffset;
                    }

                    //else, visit the first child before the second child
                    else{
                        toDo[todoOffset++] = node->secondChildOffset;
                        nodeNum = nodeNum + 1;
                    }

                }


            }

            else{
                //if the ray didn't intersect shit, get the next bounding box to check (if the're are any more bounding boxes to check)

                //getting next node to test (if there is a next node to test)
                if(todoOffset == 0){
                    break;
                }
                nodeNum = toDo[--todoOffset];
            }


        }

    }

    //after I check all the triangles with my ray,
    //see if the ray hit anything and if so, get the intersection pt.

    //if we have a valid t...
    if(tMin > 0.0f && tMin != FLT_MAX)
    {
        //...get the intersection
        Eigen::Vector3f intersectionPt = GetIntPt(ray, tMin);


        //getting the hit UV
        GetHitUV(hitTri);

        //get normal of intersection point on hit triangle
        Eigen::Vector4f hitNormal = GetHitNormal(hitTri);

        //if the ray is in the object, don't do ANY of this shit
        if(!(ray.isInObject)){

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


                    //indexing into the texture
                    float xText = invUV[0] * (float)hitObject.texture->width;
                    float yText = invUV[1] * (float)hitObject.texture->height;

                    se::math::Vector3D textCoord(xText, yText, 0.0f);

                    se::graphics::image::Color4f col = hitObject.texture->read(textCoord, FloatImageRGBA::LINEAR);


                    //adding n dot L to the color components
                    resultColor.redComp += nDotLPt * col.x * pointLights[ptLight].lightColor.redComp;
                    resultColor.greenComp += nDotLPt * col.y * pointLights[ptLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLPt * col.z * pointLights[ptLight].lightColor.blueComp;
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


                    //indexing into the texture
                    float xText = invUV[0] * (float)hitObject.texture->width;
                    float yText = invUV[1] * (float)hitObject.texture->height;

                    se::math::Vector3D textCoord(xText, yText, 0.0f);

                    se::graphics::image::Color4f col = hitObject.texture->read(textCoord, FloatImageRGBA::LINEAR);


                    //adding n dot L to the color components
                    resultColor.redComp += nDotLDir * col.x * directionalLights[dirLight].lightColor.redComp;
                    resultColor.greenComp += nDotLDir * col.y * directionalLights[dirLight].lightColor.greenComp;
                    resultColor.blueComp += nDotLDir * col.z * directionalLights[dirLight].lightColor.blueComp;
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

        }


        //getting surface normal to the triangle that I hit
        Eigen::Vector3f hitNorToTri2 = GetNormalToTri(hitTri);

        return resultColor * (1.0f - hitObject.reflection_coeff - hitObject.refraction_coeff) +
               GetColor( GetReflectionRay(ray, hitNormal, intersectionPt, hitNorToTri2) ) * hitObject.reflection_coeff +
               GetColor( GetRefractionRay(ray, hitNormal.head<3>(), intersectionPt, hitObject, hitNorToTri2) ) * hitObject.refraction_coeff;

    }

    else
    {
        //if we don't have an intersection, get a background pixel
        se::math::Vector3D v(ray.normalizedDirect);
        se::graphics::image::Color4f colBack = background->get(v);

        resultColor.redComp = colBack.x;
        resultColor.greenComp = colBack.y;
        resultColor.blueComp = colBack.z;


        return resultColor;
    }

}


//////////////////////////////////////
////////Getting the Reflection Ray////
//////////////////////////////////////
MyRay RayTracer::GetReflectionRay(MyRay incidentRay, Eigen::Vector4f normal, Eigen::Vector3f orgin, Eigen::Vector3f hitNorToTri){
    MyRay reflectionRay;
    reflectionRay.depth = incidentRay.depth;

    reflectionRay.origin = orgin + (1e-3 * hitNorToTri);

    //reflected ray equation
    reflectionRay.direction = incidentRay.direction - 2*(normal.head<3>().dot(incidentRay.direction)) * normal.head<3>();

    //doing this so I can normalize the temp and set it equal to the normalized direction of the ray
    //the reflection ray direction should already be normalized, but normalize it again anyways
    Eigen::Vector3f tempDirection = reflectionRay.direction;
    tempDirection.normalize();

    reflectionRay.normalizedDirect = tempDirection;

    return reflectionRay;

}
//////////////////////////////////////
////////Getting the Reflection Ray////
//////////////////////////////////////


//////////////////////////////////////
////////Getting the Refraction Ray////
//////////////////////////////////////
MyRay RayTracer::GetRefractionRay(MyRay incidentRay, Eigen::Vector3f normal, Eigen::Vector3f orgin, Object hitObject, Eigen::Vector3f hitNorToTri){
    MyRay refractedRay;
    refractedRay.depth = incidentRay.depth;

    refractedRay.origin = orgin;

    //now we start the long process of calculating the refracted ray's direction

    //get the reverse incident ray
    Eigen::Vector3f revIncident = -incidentRay.normalizedDirect;

    //get the dot product of this reversed incident with the normal
    float revDotNormal = revIncident.dot(normal);


    //if this dot product is less than 0, we're inside the material
    if(revDotNormal < 0){

        refractedRay.origin += (1e-3 * hitNorToTri);



        //////////////////////
        ////sumat's method////
        //////////////////////

        //getting the reverse of the normal to get the proper angle1
        Eigen::Vector3f revNormal = -normal;

        float revDotRevNormal = revIncident.dot(revNormal);

        float angle1 = acos(revDotRevNormal);

        float angle2 = Snell(hitObject.indexOfRefrac, 1.0f, angle1);


        float cosAngle1 = cos(angle1);

        //the reason I have normal's switched in terms of sign compared to the normal's of my else statement is because since
        //i'm inside the object, the normal I hit is exactly facing where I need it to face
        Eigen::Vector3f biTangent = (incidentRay.normalizedDirect + (-normal) * cosAngle1)/sqrt(1 - (cosAngle1 * cosAngle1) );

        refractedRay.direction = (biTangent * sin(angle2)) + (normal * cos(angle2));

        //////////////////////
        ////sumat's method////
        //////////////////////



        /*
        ///////////////////////
        ///glassner's method///
        ///////////////////////


        Eigen::Vector3f revNormal = -normal;
        float snellDivide = (hitObject.indexOfRefrac/1.0f);
        float cI = revNormal.dot(revIncident);

        float v = 1 + ( snellDivide * snellDivide ) * ( (cI * cI) - 1);
        refractedRay.direction = (snellDivide * incidentRay.normalizedDirect) + (snellDivide*cI - sqrt(v)) * (revNormal);


        ///////////////////////
        ///glassner's method///
        ///////////////////////
        */


        //doing this so I can normalize the temp and set it equal to the normalized direction of the ray
        //the refraction ray direction should already be normalized, but normalize it again anyways
        Eigen::Vector3f tempDirection = refractedRay.direction;
        tempDirection.normalize();

        refractedRay.normalizedDirect = tempDirection;

        //set is in object to false because we went from the inside to the outside of the material
        refractedRay.isInObject = false;
    }

    //if this dot product is NOT less than 0, we're outside the material
    else{
        refractedRay.origin -= (1e-3 * hitNorToTri);



        //////////////////////
        ////sumat's method////
        //////////////////////

        float angle1 = acos(revDotNormal);

        float angle2 = Snell(1.0f, hitObject.indexOfRefrac, angle1);


        float cosAngle1 = cos(angle1);


        Eigen::Vector3f biTangent = (incidentRay.normalizedDirect + normal * cosAngle1)/sqrt(1 - (cosAngle1 * cosAngle1) );

        refractedRay.direction = (biTangent * sin(angle2)) + ( (-normal) * cos(angle2));

        //////////////////////
        ////sumat's method////
        //////////////////////



        /*
        ///////////////////////
        ///glassner's method///
        ///////////////////////

        float snellDivide = (1.0f/hitObject.indexOfRefrac);
        float cI = normal.dot(revIncident);

        float v = 1 + ( snellDivide * snellDivide ) * ( (cI * cI) - 1);
        refractedRay.direction = (snellDivide * incidentRay.direction) + (snellDivide*cI - sqrt(v)) * (normal);


        ///////////////////////
        ///glassner's method///
        ///////////////////////
        */


        //doing this so I can normalize the temp and set it equal to the normalized direction of the ray
        //the refraction ray direction should already be normalized, but normalize it again anyways
        Eigen::Vector3f tempDirection = refractedRay.direction;
        tempDirection.normalize();

        refractedRay.normalizedDirect = tempDirection;


        //set is in object to true because we're now inside the material
        refractedRay.isInObject = true;
    }

    return refractedRay;

}


//function for snell's law
float RayTracer::Snell(float index1, float index2, float angle){
    return asin((index1 * sin(angle))/index2);
}
//////////////////////////////////////
////////Getting the Refraction Ray////
//////////////////////////////////////



/////////////////////////////////////////////////
/////////Directional Light Shadow Functions//////
/////////////////////////////////////////////////

MyRay RayTracer::GetDirShadowRay(Eigen::Vector3f intersection_pt, DirLight dirLight){
    MyRay dirShadowRay;

    //this shadow ray's origin is the intersection point we got from ray tracing
    dirShadowRay.origin = intersection_pt;

    //the direction of this shadow ray is the direction of the directional light
    dirShadowRay.direction = dirLight.dirLightL.head<3>();

    //normalize the directional lights shadow ray cause we're gonna check if the t is greater than 0
    dirShadowRay.direction.normalize();

    return dirShadowRay;

}


bool RayTracer::IsInShadowDir(MyRay dirShadowRay){
    float tDirShadow = FLT_MAX;

    //passing direction instead of normalized direction because direction is ALREADY normalized (look above, dumbass)
    Eigen::Vector3f invDir = Eigen::Vector3f( (1.0f / dirShadowRay.direction[0]), (1.0f / dirShadowRay.direction[1]), (1.0f / dirShadowRay.direction[2]) );
    Eigen::Vector3i dirIsNeg = Eigen::Vector3i(invDir[0] < 0, invDir[1] < 0, invDir[2] < 0);

    int todoOffset;
	int nodeNum;
	int toDo[64];

	int primOffset = 0;
	int secondChildOffset = 0;
	int numPrimitives = 0;
	int splitAxis = -1;


	bool isInBox = false;


    //iterating through all objects on the list
    for(int j = 0; j < BVHList.size(); j++){

        //if object is refractive, we can't be in shadow
        if(objectList[j].refraction_coeff != 0.0f){
            continue;
        }

        todoOffset = 0;
        nodeNum = 0;
        for(int k = 0; k < 64; k++){
            toDo[k] = 0;
        }

        std::vector<Triangle> & triList = BVHList[j].triList;

        while(true){
            const LinearBVHNode *node = &(BVHList[j].flatList[nodeNum]);

            numPrimitives = node->numPrimitives;

            //if i'm in the box...
            isInBox = IsInBox(node->bounds, dirShadowRay.origin);

            if( isInBox && numPrimitives == 0){
                //...always visit left child then right child
                toDo[todoOffset++] = node->secondChildOffset;
                nodeNum = nodeNum + 1;

                continue;
            }


            //checking to see if we get an intersection with the currently looked at bounding box
            if(IntersectBVHNode(node->bounds, invDir, dirIsNeg, dirShadowRay.origin, FLT_MAX) || isInBox){

                if(node->numPrimitives > 0){
                    //we hit a leaf node, so intersect with the primitives in the leaf node
                    for(int i = 0; i < node->numPrimitives; i++)
                    {
                        Eigen::Vector3f normalToTri;

                        //finding the normal to the currently looked-at triangle
                        normalToTri = GetNormalToTri(triList[i + node->primitivesOffset]);

                        //for triangles that are parallel to the ray...they'll never intersect
                        if( fabs(normalToTri.dot(dirShadowRay.direction)) <= 1e-8)
                        {
                            continue;
                        }

                        //get the t of intersection
                        tDirShadow = IsGenericHit(triList[i + node->primitivesOffset], dirShadowRay);

                        //if the t of the shadow is valid and is greater than 0...
                        if(tDirShadow > 0.0f && tDirShadow != FLT_MAX)
                        {
                            //...return true cause the point's in shadow
                            return true;
                        }

                    }

                    //getting next node to test (if there is a next node to test)
                    if(todoOffset == 0) break;
                    nodeNum = toDo[--todoOffset];

                }

                //else, we need to traverse the BVH tree more
                else{
                    //if our split axis direction was negative, visit the second child before the first child
                    if(dirIsNeg[node->axis]){
                        toDo[todoOffset++] = nodeNum + 1;
                        nodeNum = node->secondChildOffset;
                    }

                    //else, visit the first child before the second child
                    else{
                        toDo[todoOffset++] = node->secondChildOffset;
                        nodeNum = nodeNum + 1;
                    }
                }


            }

            else{
                //if the ray didn't intersect shit, get the next bounding box to check (if the're are any more bounding boxes to check)

                //getting next node to test (if there is a next node to test)
                if(todoOffset == 0) break;
                nodeNum = toDo[--todoOffset];
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

MyRay RayTracer::GetPtShadowRay(Eigen::Vector3f intersection_pt, PointLight ptLight){
    MyRay ptShadowRay;

    //this shadow ray's origin is the intersection point we got from ray tracing
    ptShadowRay.origin = intersection_pt;


    //the direction of this pt. shadow ray is from the intersection point to the light position
    //i'm NOT normalizing this direction cause i'm gonna check if t is between 0 and 1
    ptShadowRay.direction = ptLight.lightPos - intersection_pt;


    return ptShadowRay;
}

//function to test if we hit a point light source
bool RayTracer::IsInShadowPt(MyRay ptShadowRay){

    float tPtShadow = 0.0f;

    //passing direction instead of normalized direction because we want to see if t is between 0 and 1 (this is a pt. light, after all)
    Eigen::Vector3f invDir = Eigen::Vector3f( (1.0f / ptShadowRay.direction[0]), (1.0f / ptShadowRay.direction[1]), (1.0f / ptShadowRay.direction[2]) );
    Eigen::Vector3i dirIsNeg = Eigen::Vector3i(invDir[0] < 0, invDir[1] < 0, invDir[2] < 0);

    int todoOffset;
	int nodeNum;
	int toDo[64];

	int primOffset = 0;
	int secondChildOffset = 0;
	int numPrimitives = 0;
	int splitAxis = -1;

	bool isInBox = false;


    //precalculate the ptShadowRay's direction to a temp variable to test for parallelism
    Eigen::Vector3f tempShadowDir = ptShadowRay.direction;
    tempShadowDir.normalize();

    //iterating through all objects on the list
    for(int j = 0; j < BVHList.size(); j++){

        //if object is refractive, we can't be in shadow
        if(objectList[j].refraction_coeff != 0.0f){
            continue;
        }

        todoOffset = 0;
        nodeNum = 0;
        for(int k = 0; k < 64; k++){
            toDo[k] = 0;
        }

        std::vector<Triangle> & triList = BVHList[j].triList;

        while(true){
            const LinearBVHNode *node = &(BVHList[j].flatList[nodeNum]);


            numPrimitives = node->numPrimitives;

            //if i'm in the box...
            isInBox = IsInBox(node->bounds, ptShadowRay.origin);

            if( isInBox && numPrimitives == 0){
                //...always visit left child then right child
                toDo[todoOffset++] = node->secondChildOffset;
                nodeNum = nodeNum + 1;

                continue;
            }

            //checking to see if we get an intersection with the currently looked at bounding box
            if(IntersectBVHNode(node->bounds, invDir, dirIsNeg, ptShadowRay.origin, 1.0f) || isInBox){

                if(node->numPrimitives > 0){
                    //we hit a leaf node, so intersect with the primitives in the leaf node
                    for(int i = 0; i < node->numPrimitives; i++)
                    {
                        Eigen::Vector3f normalToTri;

                        //finding the normal to the currently looked-at triangle
                        normalToTri = GetNormalToTri(triList[i + node->primitivesOffset]);

                        //for triangles that are parallel to the ray...they'll never intersect
                        if( fabs(normalToTri.dot(tempShadowDir)) <= 1e-8)
                        {
                            continue;
                        }

                        //get the t of intersection
                        tPtShadow = IsGenericHit(triList[i + node->primitivesOffset], ptShadowRay);

                        //if the t is valid and less than the currently stored t...
                        if(tPtShadow > 0.0f && tPtShadow < 1.0f)
                        {
                            //...return true cause the point's in shadow
                            return true;
                        }

                    }

                    //getting next node to test (if there is a next node to test)
                    if(todoOffset == 0) break;
                    nodeNum = toDo[--todoOffset];

                }

                //else, we need to traverse the BVH tree more
                else{
                    //if our split axis direction was negative, visit the second child before the first child
                    if(dirIsNeg[node->axis]){
                        toDo[todoOffset++] = nodeNum + 1;
                        nodeNum = node->secondChildOffset;
                    }

                    //else, visit the first child before the second child
                    else{
                        toDo[todoOffset++] = node->secondChildOffset;
                        nodeNum = nodeNum + 1;
                    }
                }


            }

            else{
                //if the ray didn't intersect shit, get the next bounding box to check (if the're are any more bounding boxes to check)

                //getting next node to test (if there is a next node to test)
                if(todoOffset == 0) break;
                nodeNum = toDo[--todoOffset];
            }


        }


    }


    //if we got this far then we're not in shadow
    return false;

}

/////////////////////////////////////////////
/////////Point Light Shadow Functions////////
/////////////////////////////////////////////



//this function gives you back a ray that will be shot to find intersection
MyRay RayTracer::GetOriginRay(int j, int i)
{
    Eigen::Vector3f orgin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

    Eigen::Vector3f direct;

    //the x-coord of direction
    direct[0] = i * (width_of_pixel) + uL_x;

    //the y-coord of direction
    direct[1] = -j * (height_of_pixel) + uL_y;

    //the z-coord of direction
    direct[2] = -near_plane;

    //copying direction vector so I can normalize it and pass it on to the ray constructor
    Eigen::Vector3f normalDirect = direct;
    normalDirect.normalize();

    //creating a ray with a depth of zero
    MyRay testRay(orgin, direct, normalDirect, 0, 0);

    return testRay;
}




float RayTracer::IsHit(Triangle tri, MyRay testRay)
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


    mat1 << (tri.vertices[1].position[0] - tri.vertices[2].position[0]), (tri.vertices[0].position[0] - tri.vertices[2].position[0]), (-testRay.normalizedDirect[0]),
            (tri.vertices[1].position[1] - tri.vertices[2].position[1]), (tri.vertices[0].position[1] - tri.vertices[2].position[1]), (-testRay.normalizedDirect[1]),
            (tri.vertices[1].position[2] - tri.vertices[2].position[2]), (tri.vertices[0].position[2] - tri.vertices[2].position[2]), (-testRay.normalizedDirect[2]);



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

            baryTemp.alphaTemp = alphaTemp;
            baryTemp.betaTemp = betaTemp;
            baryTemp.gammaTemp = gammaTemp;

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


//this IsHit function doesn't fuck around with baryTemp
//it's used for the shadow functions
float RayTracer::IsGenericHit(Triangle tri, MyRay testRay)
{
    //log << "isgenerichit testRay direction: " << testRay.direction << endl;


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



//return the intersection point
Eigen::Vector3f RayTracer::GetIntPt(MyRay testRay, float t)
{
    Eigen::Vector3f intersectionPt;
    intersectionPt[0] = testRay.origin[0] + testRay.normalizedDirect[0] * t;
    intersectionPt[1] = testRay.origin[1] + testRay.normalizedDirect[1] * t;
    intersectionPt[2] = testRay.origin[2] + testRay.normalizedDirect[2] * t;
    return intersectionPt;
}


//return the UV
void RayTracer::GetHitUV(Triangle hitTri)
{

    for(int i = 0; i < 2; i++)
    {
        invUV[i] = (hitTri.vertices[2].textCoord[i] * alpha) + (hitTri.vertices[1].textCoord[i] * beta) + (hitTri.vertices[0].textCoord[i] * gamma);
    }


    //log << "u: " << invUV[0] << endl;
    //log << "v: " << invUV[1] << endl;

}



//getting the normalized normal of the intersection pt of the triangle
Eigen::Vector4f RayTracer::GetHitNormal(Triangle hitTri)
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
Eigen::Vector3f RayTracer::GetPoint(Triangle hitTri)
{
    Eigen::Vector3f realPt;

    for(int i = 0; i < 3; i++)
    {
        realPt[i] = (hitTri.vertices[2].position[i] * alpha) + (hitTri.vertices[1].position[i]  * beta) + (hitTri.vertices[0].position[i] * gamma);
    }

    return realPt;

}


//getting the normal to the triangle i'm looking at, used for parallel testing
Eigen::Vector3f RayTracer::GetNormalToTri(Triangle tri)
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


//returning the distance between two points
double RayTracer::Distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2)
{
    double diffX, diffY, diffZ;
    diffX = pt1[0] - pt2[0];
    diffY = pt1[1] - pt2[1];
    diffZ = pt1[2] - pt2[2];

    return sqrt( (diffX * diffX) + (diffY * diffY) + (diffZ * diffZ) );
}

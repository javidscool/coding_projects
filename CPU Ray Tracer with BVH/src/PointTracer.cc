#include "PointTracer.h"
#include<se/graphics/image/formats/RgbeImage.h>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace se::graphics::image;

//ray-tracer constructor
PointTracer::PointTracer(std::vector<PointObject> objList, const int width, const int height)
{
    //set the object list
    objectList = objList;

    zAxis = Eigen::Vector3f::Zero(3);
    yAxis = Eigen::Vector3f::Zero(3);
    xAxis = Eigen::Vector3f::Zero(3);


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

    //setting up the background
    ifstream bgf("bg.hdr", ifstream::in | ifstream::binary);
    formats::RgbeImage bgim(bgf);
    background.reset(new st::image::pano::LatitudeLongitude(bgim));


    //used for visualization of intersection points
    hitSuccessB = false;
}



//this is the ray bounding box intersection to check if the ray intersects the bounding box of the point object
//I return tMin
float PointTracer::IntersectBoundingBox(const AABBox &bounds, Eigen::Vector3f invDir, Eigen::Vector3i dirIsNeg, Eigen::Vector3f rayOrig, float rayMaxt){
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
		//return crap if I fail
		return -100.0f;
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
		//return crap if I fail
		return -100.0f;
	}

    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;


	if( (tMin < rayMaxt) && (tMax > 0.0f) ){
	    return tMin;
	}
}


bool PointTracer::IsInBox(const AABBox &bounds, Eigen::Vector3f pt){
    return (pt[0] >= bounds.pMin[0] && pt[0] <= bounds.pMax[0] &&
            pt[1] >= bounds.pMin[1] && pt[1] <= bounds.pMax[1] &&
            pt[2] >= bounds.pMin[2] && pt[2] <= bounds.pMax[2]);
}

//MAIN FUNCTION THAT DOES EVERYTHING
Color PointTracer::GetColor(MyRay ray)
{
    //initializing to the maximum float
    float tMin = FLT_MAX;

    float tTemp = 0.0f;

    //centroid is a vector3f that's initialized to zero
    Eigen::Vector3f centroid = Eigen::Vector3f::Zero(3);
    float centroidT = 0.0f;

    float bboxDiag;
    float h;

    Eigen::Vector3f intersectionPt;

    Color resultColor;

    PointObject hitObject;

    Eigen::Vector3f invDir = Eigen::Vector3f( (1.0f / ray.normalizedDirect[0]), (1.0f / ray.normalizedDirect[1]), (1.0f / ray.normalizedDirect[2]) );
    Eigen::Vector3i dirIsNeg = Eigen::Vector3i( (invDir[0] < 0), (invDir[1] < 0), (invDir[2] < 0) );

    bool isInBox = false;

    bool isHitObject;
    bool rayMiss;

    bool hitSuccess = false;

    float rayHitDist;


    //increase the ray depth
    ray.depth++;

    //if the ray has gone deep enough, return zero color through the recursion
    if(ray.depth == 8){
        return resultColor;
    }


    for(int j = 0; j < objectList.size(); j++){
        //log << "number of points in this object: " << objectList[j].pointList.size() << endl;
        isHitObject = false;
        rayMiss = false;

        //raytrace the bounding box for the current ray and see if you've got a hit
        centroidT = IntersectBoundingBox(objectList[j].boundingBox, invDir, dirIsNeg, ray.origin, tMin);
        //log << "centroidT: " << centroidT << endl;

        //if i've got a hit on the bounding box
        if(centroidT > 0.0f){
            //log << "I hit the bounding box... time to work" << endl;

            //calculating our centroid point using the centroidT, this is our starting point
            centroid = GetIntPt(ray, centroidT);
            //log << "centroid: " << centroid << endl;

            //set the ray's origin equal to the centroid
            //tempRay.origin = centroid;

            bboxDiag = Distance(objectList[j].boundingBox.pMin, objectList[j].boundingBox.pMax);
            //log << "bbox diag: " << bboxDiag << endl;
            //the radius of influence
            h = 0.004*bboxDiag;

            MyRay tempRay;
            //iterating until we hit the object or fail by missing the object
            while(isHitObject == false && rayMiss == false){
                //first get the coord axis
                SetCoordSystem(objectList[j], centroid, h);

                //then create the curve using that coordinate axis
                CreateCurve(objectList[j], centroid, h);

                tempRay = GetLocalRay(ray);

                //now check for a hit
                tTemp = IsHit(tempRay);
                //log << "tTemp: " << tTemp << endl;
                //if the t is valid and less than the currently stored t...
                if(tTemp > 0.0f && tTemp < tMin)
                {
                    //..get the new t
                    tMin = tTemp;
                    //log << "tMin: " << tMin << endl;

                    //get the object that was hit
                    hitObject = objectList[j];



                    //since we got an intersection I need to move my ray origin up to the intersection point of the curve
                    Eigen::Vector3f intersectionPtTemp = GetIntPt(tempRay, tMin);

                    //log << "intersection temp: " << intersectionPtTemp << endl;

                    //log << "centroid: " << centroid << endl;

                    //get the distance from the ray's origin to the new intersection point
                    rayHitDist = Distance(tempRay.origin, intersectionPtTemp);

                    //if our intersection point is basically our origin...
                    if(rayHitDist <= 1e-3){
                        log << "We got a hit!!!!!" << endl;
                        //...then we have a REAL intersection point
                        isHitObject = true;
                        intersectionPt = intersectionPtTemp;

                        hitSuccess = true;

                    }

                    //else we didn't get an intersection
                    else{
                        //log << "iterating again" << endl;

                        //get the new centroid (add old centroid to get centroid back in world space)
                        Eigen::Vector3f centroidTemp = intersectionPtTemp[0] * xAxis + intersectionPtTemp[1] * yAxis + intersectionPtTemp[2] * zAxis;
                        centroidTemp += centroid;

                        centroid = centroidTemp;
                    }

                }

                else{
                    //log << "done iterating...ray missed" << endl;

                    //else our ray missed the curve
                    rayMiss = true;

                    hitSuccess = false;
                }
            }

        }

        else{
            //log << "I didn't hit the bounding box" << endl;
        }

    }


    //see if the ray hit anything and if so, get the intersection pt.

    //if we have a valid t...
    if(tMin > 0.0f && tMin != FLT_MAX && hitSuccess)
    {
        //log << "printing red" << endl;

        resultColor.redComp = 1.0f;

        return resultColor;
    }

    else
    {
        //log << "printing background" << endl;

        /*
        //if we don't have an intersection, get a background pixel
        se::math::Vector3D v(ray.normalizedDirect);
        se::graphics::image::Color4f colBack = background->get(v);

        resultColor.redComp = colBack.x;
        resultColor.greenComp = colBack.y;
        resultColor.blueComp = colBack.z;
        */

        resultColor.redComp = 0.0f;
        resultColor.greenComp = 1.0f;
        resultColor.blueComp = 1.0f;

        return resultColor;
    }

}

//function used to visualize intersection points
std::vector<Eigen::Vector3f> PointTracer::GetRayPoints(PointObject &object, MyRay ray){
    std::vector<Eigen::Vector3f> result;

    //initializing to the maximum float
    float tMin = FLT_MAX;

    float tTemp = 0.0f;

    //centroid is a vector3f that's initialized to zero
    Eigen::Vector3f centroid = Eigen::Vector3f::Zero(3);
    float centroidT = 0.0f;

    float bboxDiag;
    float h;

    Eigen::Vector3f intersectionPt;

    Color resultColor;

    PointObject hitObject;

    Eigen::Vector3f invDir = Eigen::Vector3f( (1.0f / ray.normalizedDirect[0]), (1.0f / ray.normalizedDirect[1]), (1.0f / ray.normalizedDirect[2]) );
    Eigen::Vector3i dirIsNeg = Eigen::Vector3i( (invDir[0] < 0), (invDir[1] < 0), (invDir[2] < 0) );

    bool isInBox = false;

    bool isHitObject;
    bool rayMiss;

    float rayHitDist;



    //log << "number of points in this object: " << objectList[j].pointList.size() << endl;
    isHitObject = false;
    rayMiss = false;

    //raytrace the bounding box for the current ray and see if you've got a hit
    centroidT = IntersectBoundingBox(object.boundingBox, invDir, dirIsNeg, ray.origin, tMin);
    //log << "centroidT: " << centroidT << endl;

    //if i've got a hit on the bounding box
    if(centroidT > 0.0f){
        //log << "I hit the bounding box... time to work" << endl;

        //calculating our centroid point using the centroidT, this is our starting point
        centroid = GetIntPt(ray, centroidT);

        result.push_back(centroid);

        //log << "centroid: " << centroid << endl;

        //set the ray's origin equal to the centroid
        //tempRay.origin = centroid;

        bboxDiag = Distance(object.boundingBox.pMin, object.boundingBox.pMax);
        //log << "bbox diag: " << bboxDiag << endl;
        //the radius of influence
        h = 0.004*bboxDiag;

        //log << "creating influence: " << influence << endl;


        MyRay tempRay;
        //iterating until we hit the object or fail by missing the object
        while(isHitObject == false && rayMiss == false && hitSuccessB == false){
            //first get the coord axis
            SetCoordSystem(object, centroid, h);

            //then create the curve using that coordinate axis
            CreateCurve(object, centroid, h);

            tempRay = GetLocalRay(ray);

            //now check for a hit
            tTemp = IsHit(tempRay);
            //log << "tTemp: " << tTemp << endl;
            //if the t is valid and less than the currently stored t...
            if(tTemp > 0.0f && tTemp < tMin)
            {
                //..get the new t
                tMin = tTemp;
                //log << "tMin: " << tMin << endl;

                //get the object that was hit
                //hitObject = objectList[j];



                //since we got an intersection I need to move my ray origin up to the intersection point of the curve
                Eigen::Vector3f intersectionPtTemp = GetIntPt(tempRay, tMin);

                //log << "intersection temp: " << intersectionPtTemp << endl;

                //log << "centroid: " << centroid << endl;

                //get the distance from the ray's origin to the new intersection point
                rayHitDist = Distance(tempRay.origin, intersectionPtTemp);

                //if our intersection point is basically our origin...
                if(rayHitDist <= 1e-3){
                    log << "We got a hit!!!!!" << endl;
                    //...then we have a REAL intersection point
                    isHitObject = true;
                    intersectionPt = intersectionPtTemp;

                    hitSuccessB = true;

                }

                //else we didn't get an intersection
                else{
                    //log << "iterating again" << endl;

                    //get the new centroid (add old centroid to get centroid back in world space)
                    Eigen::Vector3f centroidTemp = intersectionPtTemp[0] * xAxis + intersectionPtTemp[1] * yAxis + intersectionPtTemp[2] * zAxis;
                    centroidTemp += centroid;

                    centroid = centroidTemp;

                    result.push_back(centroid);
                }

            }

            else{
                //log << "done iterating...ray missed" << endl;

                //else our ray missed the curve
                rayMiss = true;

                hitSuccessB = false;
            }
        }

    }

    else{
        //log << "I didn't hit the bounding box" << endl;
    }




    //see if the ray hit anything and if so, get the intersection pt.

    //if we have a valid t...
    if(tMin > 0.0f && tMin != FLT_MAX && hitSuccessB)
    {
        Eigen::Vector3f centroidTemp = intersectionPt[0] * xAxis + intersectionPt[1] * yAxis + intersectionPt[2] * zAxis;
        centroidTemp += centroid;

        centroid = centroidTemp;
        result.push_back(centroid);

        return result;
    }

    else
    {
        //log  << "I'm returning crap" << endl;
        Eigen::Vector3f crap(0.0f, 0.0f, 0.0f);

        return result;
    }
}


MyRay PointTracer::GetLocalRay(MyRay ray){
    MyRay temp;

    temp.origin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

    temp.direction[0] = ray.direction.dot(xAxis);
    temp.direction[1] = ray.direction.dot(yAxis);
    temp.direction[2] = ray.direction.dot(zAxis);

    Eigen::Vector3f normalTemp = temp.direction;
    normalTemp.normalize();

    temp.normalizedDirect = normalTemp;

    return temp;


}


void PointTracer::SetCoordSystem(PointObject object, Eigen::Vector3f centroid, float h){

    float d;

    //float bboxDiag;
    //float hell;

    //bboxDiag = Distance(object.boundingBox.pMin, object.boundingBox.pMax);
    //log << "bbox diag: " << bboxDiag << endl;
    //the radius of influence
    //hell = 0.004*bboxDiag;

    //the weight matrix (initialized to all zeroes)
    //Eigen::MatrixXf weightMatrix(object.pointList.size(), object.pointList.size());
    //weightMatrix = Eigen::MatrixXf::Zero(object.pointList.size(), object.pointList.size());


    //the matrix (pt[i] - centroid)
    Eigen::MatrixXf coordSystem(object.pointList.size(), 3);



    for(int i = 0; i < object.pointList.size(); i++){
        //pt[i] - centroid
        Eigen::Vector3f newRow = (object.pointList[i].position - centroid);
        coordSystem.row(i) = newRow;


        //setting a new element in the weight matrix
        //d = Distance(object.pointList[i].position, centroid);
        //log << "My d: " << d << endl;
        //log << "My h: " << h << endl;

        //weightMatrix(i, i) = exp( -( (d * d) / (hell * hell) ) );
        //weightMatrix(i, i) = exp( -( (d * d) / (h * h) ) );

        //log << "LALALALALA: " << weightMatrix(i, i) << endl;
    }

    //getting the transpose of the coordSystem matrix
    Eigen::MatrixXf coordSystemTranspose(3, object.pointList.size());
    coordSystemTranspose = coordSystem.transpose();

    //now get the covariance matrix by multiplying transpose * weight * coordSystem
    //Eigen::Matrix3f covMatrix = coordSystemTranspose * weightMatrix * coordSystem;
    Eigen::Matrix3f covMatrix = coordSystemTranspose * coordSystem;

    //now we need the eigenvectors because they are our new coordinate system
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigensolver(covMatrix);

    zAxis = eigensolver.eigenvectors().col(0);
    yAxis = eigensolver.eigenvectors().col(1);
    xAxis = eigensolver.eigenvectors().col(2);

    //log << "zAxis: " << zAxis << endl;
    //log << "yAxis: " << yAxis << endl;
    //log << "xAxis: " << xAxis << endl;
}


void PointTracer::CreateCurve(PointObject &object, Eigen::Vector3f centroid, float h){
    float d;

    //float bboxDiag;
    //float hell;

    //bboxDiag = Distance(object.boundingBox.pMin, object.boundingBox.pMax);
    //log << "bbox diag: " << bboxDiag << endl;
    //the radius of influence
    //hell = 0.004*bboxDiag;

    //the weight matrix (initialized to all zeroes)
    //Eigen::MatrixXf weightMatrix(object.pointList.size(), object.pointList.size());
    //weightMatrix = Eigen::MatrixXf::Zero(object.pointList.size(), object.pointList.size());


    std::vector<Point> modPtList;

    //the matrix with the x and y information
    Eigen::MatrixXf xyInfo(object.pointList.size(), 6);

    //the matrix with the z information
    Eigen::MatrixXf zInfo(object.pointList.size(), 1);


    Eigen::Vector3f ptVect;

    float x, y, z;

    //creating my matrices
    for(int i = 0; i < object.pointList.size(); i++){
        //getting p[i] - centroid
        ptVect = object.pointList[i].position - centroid;

        x = ptVect.dot(xAxis);
        y = ptVect.dot(yAxis);
        z = ptVect.dot(zAxis);

        Eigen::Vector3f point(x, y, z);
        //log << "modded point: " << point << endl;
        Point pt;
        pt.Equals(point);

        modPtList.push_back(pt);

        Eigen::Matrix<float, 1, 6> xyRow;
        xyRow << (x * x), (y * y), (x * y), x, y, 1;
        xyInfo.row(i) = xyRow;

        zInfo(i,0) = z;



        //setting a new element in the weight matrix
        //d = Distance(object.pointList[i].position, centroid);
        //weightMatrix(i, i) = exp( -( (d * d) / (hell * hell) ) );
        //weightMatrix(i, i) = exp( -( (d * d) / (h * h) ) );
    }

    //now get the transpose of xyInfo
    Eigen::MatrixXf xyInfoTranspose(6, object.pointList.size());
    xyInfoTranspose = xyInfo.transpose();

    //Eigen::Matrix<float, 6, 6> temp1 = (xyInfoTranspose * weightMatrix * xyInfo);
    Eigen::Matrix<float, 6, 6> temp1 = (xyInfoTranspose * xyInfo);

    Eigen::Matrix<float, 6, 6> temp2 = temp1.inverse();

    //Eigen::Matrix<float, 6, 1> temp3 = (xyInfoTranspose * weightMatrix * zInfo);
    Eigen::Matrix<float, 6, 1> temp3 = (xyInfoTranspose * zInfo);


    curve = temp2 * temp3;

    object.modPointList = modPtList;

    //log << "curve I get: " << curve << endl;
}

std::vector<Eigen::Vector3f> PointTracer::GetCurvePoints(PointObject &object, Eigen::Vector3f centroid){
    std::vector<Eigen::Vector3f> newPoints;

    for(int i = 0; i < object.modPointList.size(); i++){
        Eigen::Vector3f point;

        point[0] = object.modPointList[i].position[0];
        point[1] = object.modPointList[i].position[1];

        point[2] = curve(0,0) * (point[0] * point[0]) + curve(1,0) * (point[1] * point[1]) + curve(2,0) * (point[0] * point[1]) + curve(3,0)*point[0] + curve(4,0)*point[1] + curve(5,0);


        point = point[0] * xAxis + point[1] * yAxis + point[2] * zAxis;

        //adding centroid to get back to world space from local space
        point += centroid;

        newPoints.push_back(point);
    }

    return newPoints;
}


float PointTracer::IsHit(MyRay testRay)
{
    //log << "curve I'm using: " << curve << endl;

    float Aq = ( curve(0,0) * (testRay.normalizedDirect[0] * testRay.normalizedDirect[0]) ) + ( curve(1,0) * (testRay.normalizedDirect[1] * testRay.normalizedDirect[1]) ) + ( curve(2,0) * (testRay.normalizedDirect[0] * testRay.normalizedDirect[1]) );

    float Bq = (2 * curve(0,0) * testRay.origin[0] * testRay.normalizedDirect[0]) + (2 * curve(1,0) * testRay.origin[1] * testRay.normalizedDirect[1]) + ( curve(2,0) * ( (testRay.origin[0] * testRay.normalizedDirect[1]) + (testRay.origin[1] * testRay.normalizedDirect[0]) ) ) + ( curve(3,0) * testRay.normalizedDirect[0] ) + ( curve(4,0) * testRay.normalizedDirect[1] );

    float Cq = ( curve(0,0) * (testRay.origin[0] * testRay.origin[0]) ) + ( curve(1,0) * (testRay.origin[1] * testRay.origin[1]) ) + ( curve(2,0) * testRay.origin[0] * testRay.origin[1] ) + ( curve(3,0) * testRay.origin[0] ) + ( curve(4,0) * testRay.origin[1] ) + curve(5,0);

    if(Aq == 0){
        return -(Cq / Bq);
    }

    else{
        //checking the squared discriminant
        float sqDis = (Bq * Bq) - 4 * Aq * Cq;
        if(sqDis < 0.0f){
            //then no intersection takes place...so return crap
            return -100.0f;
        }

        //else let's find t0
        float t0 = ( ( -Bq - sqrt(sqDis) ) / (2 * Aq) );
        //log << "t0: " << t0 << endl;
        if(t0 > 0.0f){
            //we have out t
            return t0;
        }
        else{
            float t1 = ( ( -Bq + sqrt(sqDis) ) / (2 * Aq) );
            //log << "t1: " << t1 << endl;
            if(t1 > 0.0f){
                return t1;
            }
            else{
                //we didn't hit shit so return -100
                return -100.0f;
            }
        }
    }
}


//this function gives you back a ray that will be shot to find intersection
MyRay PointTracer::GetOriginRay(int j, int i)
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


//return the intersection point
Eigen::Vector3f PointTracer::GetIntPt(MyRay testRay, float t)
{
    Eigen::Vector3f intersectionPt;
    intersectionPt[0] = testRay.origin[0] + testRay.normalizedDirect[0] * t;
    intersectionPt[1] = testRay.origin[1] + testRay.normalizedDirect[1] * t;
    intersectionPt[2] = testRay.origin[2] + testRay.normalizedDirect[2] * t;
    return intersectionPt;
}


//returning the distance between two points
float PointTracer::Distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2)
{
    float diffX, diffY, diffZ;
    diffX = pt1[0] - pt2[0];
    diffY = pt1[1] - pt2[1];
    diffZ = pt1[2] - pt2[2];

    return sqrt( (diffX * diffX) + (diffY * diffY) + (diffZ * diffZ) );
}

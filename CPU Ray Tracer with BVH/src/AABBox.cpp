#include "AABBox.h"

//constructors
AABBox::AABBox(){
    pMin = Eigen::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
    pMax = Eigen::Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}


AABBox::AABBox(const Eigen::Vector3f &p) : pMin(p), pMax(p) { }

//getting the min and max of the triangle points to make our Axis-aligned bounding box
AABBox::AABBox(const Eigen::Vector3f &point1, Eigen::Vector3f &point2, Eigen::Vector3f &point3){
    pMin = Eigen::Vector3f(min( min(point1[0], point2[0]), point3[0]), min( min(point1[1], point2[1]), point3[1]), min( min(point1[2], point2[2]), point3[2]));
    pMax = Eigen::Vector3f(max( max(point1[0], point2[0]), point3[0]), max( max(point1[1], point2[1]), point3[1]), max( max(point1[2], point2[2]), point3[2]));
}



//friend function that creates a new bounding box that contains the original bounding box and a point
AABBox Union(const AABBox &box, const Eigen::Vector3f &point){
    AABBox start = box;

    start.pMin[0] = min(box.pMin[0], point[0]);
    start.pMin[1] = min(box.pMin[1], point[1]);
    start.pMin[2] = min(box.pMin[2], point[2]);

    start.pMax[0] = max(box.pMax[0], point[0]);
    start.pMax[1] = max(box.pMax[1], point[1]);
    start.pMax[2] = max(box.pMax[2], point[2]);

    return start;
}

//friend function that creates a new bounding box that contains the two passed in bounding boxes
AABBox Union(const AABBox &box1, const AABBox &box2){
    AABBox start;

    start.pMin[0] = min(box1.pMin[0], box2.pMin[0]);
    start.pMin[1] = min(box1.pMin[1], box2.pMin[1]);
    start.pMin[2] = min(box1.pMin[2], box2.pMin[2]);

    start.pMax[0] = max(box1.pMax[0], box2.pMax[0]);
    start.pMax[1] = max(box1.pMax[1], box2.pMax[1]);
    start.pMax[2] = max(box1.pMax[2], box2.pMax[2]);

    return start;
}

//does box overlap the current bounding box?
bool AABBox::isOverlap(const AABBox &box){
    bool x = (pMax[0] >= box.pMin[0]) && (pMin[0] <= box.pMax[0]);
    bool y = (pMax[1] >= box.pMin[1]) && (pMin[1] <= box.pMax[1]);
    bool z = (pMax[2] >= box.pMin[2]) && (pMin[2] <= box.pMax[2]);

    return (x && y && z);
}

//is point inside the box?
bool AABBox::isInside(const Eigen::Vector3f &point){
    return (point[0] >= pMin[0] && point[0] <= pMax[0] &&
            point[1] >= pMin[1] && point[1] <= pMax[1] &&
            point[2] >= pMin[2] && point[2] <= pMax[2]);
}

//expand the box by a constant amount in all 3 axes
void AABBox::Expand(float delta){
    pMin -= Eigen::Vector3f(delta, delta, delta);
    pMax += Eigen::Vector3f(delta, delta, delta);
}

//computing the surface area for the box
float AABBox::SurfaceArea() const{
    Eigen::Vector3f extent = pMax - pMin;

    return 2.0f * (extent[0] * extent[1] + extent[0] * extent[2] + extent[1] * extent[2]);
}

//getting the volume of the box
float AABBox::Volume() const{
    Eigen::Vector3f extent = pMax - pMin;

    return extent[0] * extent[1] * extent[2];
}

//used for determining where to partition the primitives
//gives whether the maximum extent is the x-axis, y-axis, or z-axis
int AABBox::MaximumExtent() const {
    Eigen::Vector3f extent = pMax - pMin;

    if(extent[0] > extent[1] && extent[0] > extent[2])
        return 0;
    else if(extent[1] > extent[2])
        return 1;
    else
        return 2;
}

//what's the offset of pt from the pMin of the AABBox?
//IOW: how much is this point in the bounding box for all 3 axes?
Eigen::Vector3f AABBox::Offset(const Eigen::Vector3f &pt) const{
    Eigen::Vector3f extent = pMax - pMin;
    return Eigen::Vector3f( (pt[0] - pMin[0]) / extent[0],
                            (pt[1] - pMin[1]) / extent[1],
                            (pt[2] - pMin[2]) / extent[2]);
}


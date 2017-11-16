#ifndef AABBOX_H
#define AABBOX_H

#include <iostream>
#include <algorithm>
#include <Eigen/Geometry>
#include <cfloat>

using namespace std;

class AABBox{
    public:
        //the min and max points of the bounding box
        Eigen::Vector3f pMin;
        Eigen::Vector3f pMax;

        AABBox();
        AABBox(const Eigen::Vector3f &p);
        AABBox(const Eigen::Vector3f &point1, Eigen::Vector3f &point2, Eigen::Vector3f &point3);

        friend AABBox Union(const AABBox &box, const Eigen::Vector3f &point);
        friend AABBox Union(const AABBox &box1, const AABBox &box2);

        //does box overlap the current bounding box?
        bool isOverlap(const AABBox &box);

        //is point inside the box
        bool isInside(const Eigen::Vector3f &point);

        //expand the box by a constant amount in all 3 axes
        void Expand(float delta);

        float SurfaceArea() const;

        float Volume() const;

        //used for determining where to partition the primitives
        int MaximumExtent() const;

        //what's the offset of pt from the pMin of the AABBox
        Eigen::Vector3f Offset(const Eigen::Vector3f &pt) const;


        inline bool operator==(const AABBox &b) const {
            return b.pMin == pMin && b.pMax == pMax;
        }

        inline bool operator!=(const AABBox &b) const {
            return b.pMin != pMin || b.pMax != pMax;
        }

};

#endif

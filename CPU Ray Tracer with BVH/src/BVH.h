#ifndef BVH_H
#define BVH_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <se/core/CopyCounter.h>
#include <fstream>
#include <Eigen/Geometry>

#include "Mesh.h"
#include "AABBox.h"
#define log se::core::Log::status()

using namespace std;

//this is how primitives are represented in the BVH tree
struct BVHPrimitive{
    int primNum;
    Eigen::Vector3f centroid;
    AABBox bounds;

    BVHPrimitive(int prim_num, const AABBox &box) : primNum(prim_num), bounds(box){
        centroid = 0.5f * box.pMin + 0.5f * box.pMax;
    }
};


//each BVH node represents a node in the BVH tree
class BVHNode{
    public:
        int level;

        static int counter;

        //strores the bounds of all the children beneath or the primitives in the leaf
        AABBox bounds;

        //the left and right child of the BVHNode
        BVHNode *children[2];

        int splitAxis, firstPrimOffset, numPrim;

        BVHNode(int lev);

        ~BVHNode();

        void CreateLeaf(int first, int nPrim, const AABBox &box);

        //each interior node stores pointers to its two children
        void CreateInterior(int axis, BVHNode *c0, BVHNode *c1);
};

struct LinearBVHNode{
    AABBox bounds;

    int primitivesOffset; //for leaf
    int secondChildOffset; //for interior nodes

    //for leaves i'm storing the offset and the primitive count
    int numPrimitives; //this variable is 0 for interior nodes
    int axis;   //what coordinate axes were the primitives partitioned along when the BVH was built
};

class BVH{
    private:
        se::core::CopyCounter copyCount;
        //totalNodes is a pointer because we want everything pointing to the same totalNodes variable (we DON'T want to pass by value)
        BVHNode * buildBVH(vector<BVHPrimitive> &primList, int start, int end, int *totalNodes, vector<Triangle> &orderedTriangles, int curLevel);
        //offset is a pointer for the same reason as totalNodes is a pointer
        int compactBVH(BVHNode * node, int *offset);



        BVHNode *root;

    public:
        vector<Triangle> triList;
        LinearBVHNode *flatList;
        int totalShit;

        int nodeCount;

        BVH(vector<Triangle> &tList);

        //BBox WorldBounds() const;
        inline bool CanIntersect() const{
            return true;
        }

        ~BVH();

};

#endif

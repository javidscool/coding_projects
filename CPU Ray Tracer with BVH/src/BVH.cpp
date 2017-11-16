#include "BVH.h"
int BVHNode::counter = 0;

struct ComparePoints{
    ComparePoints(int d) { dim = d; }
    int dim;
    bool operator()(const BVHPrimitive &a,
                    const BVHPrimitive &b) const {
        return a.centroid[dim] < b.centroid[dim];
    }
};

struct CompareToBucket{
    int splitBucket, nBuckets, dim;
    const AABBox &centroidBnds;

    CompareToBucket(int split, int num, int d, const AABBox &b) : centroidBnds(b){
        splitBucket = split;
        nBuckets = num;
        dim = d;
    }

    bool operator()(const BVHPrimitive &p) const{
        int bucketDecision = nBuckets * ( (p.centroid[dim] - centroidBnds.pMin[dim]) /
                                            (centroidBnds.pMax[dim] - centroidBnds.pMin[dim]));
        //if we're at the outer border, go back one bucket
        if(bucketDecision == nBuckets){
            bucketDecision = nBuckets - 1;
        }

        return bucketDecision <= splitBucket;
    }

};

struct BucketInfo{
    int count;
    AABBox bounds;

    BucketInfo(){
        count = 0;
    }
};


////////////////
//BVHNode Shit//
////////////////
BVHNode::BVHNode(int lev){
    level = lev;
    children[0] = children[1] = NULL;
    counter++;
}

BVHNode::~BVHNode(){
    //deleting left child
    if(children[0] != NULL){
        delete children[0];
    }

    //deleting right child
    if(children[1] != NULL){
        delete children[1];
    }
}

void BVHNode::CreateLeaf(int first, int nPrim, const AABBox &box){
    firstPrimOffset = first;
    numPrim = nPrim;
    bounds = box;
}

//each interior node stores pointers to its two children
void BVHNode::CreateInterior(int axis, BVHNode *c0, BVHNode *c1){
    children[0] = c0;
    children[1] = c1;

    //the bounds of this interior node are the bounds of the two children
    bounds = Union(c0->bounds, c1->bounds);

    splitAxis = axis;

    //interior nodes don't have primitives in them...only leaf nodes do
    numPrim = 0;
}



////////////
//BVH Shit//
////////////

BVH::BVH(vector<Triangle> &tList){

    triList = tList;

    //if we don't get any triangles
    if(triList.size() == 0){
        flatList = NULL;
        return;
    }

    //now we'll build the BVH

    vector<BVHPrimitive> primData;
    primData.reserve(triList.size());

    for(int i = 0; i < triList.size(); i++){
        AABBox bound_box(triList[i].vertices[0].position, triList[i].vertices[1].position, triList[i].vertices[2].position);
        primData.push_back(BVHPrimitive(i, bound_box));
    }

    //keeping track of the total number of BVH nodes that have been created
    int totalNodes = 0;

    vector<Triangle> orderedTris;
    orderedTris.reserve(triList.size());

    //build from the start (0) to the end (triList.size) a BVH
    root = buildBVH(primData, 0, triList.size(), &totalNodes, orderedTris, -1);

    //the primitive list is swapped out with the ordered primitive list that the BVH has created
    triList.swap(orderedTris);

    //now that we have the BVH let's flatten it into a 1-D array (in depth-first order)
    flatList = new LinearBVHNode[totalNodes];

    totalShit = totalNodes;

    log << "total shit: " << totalShit << endl;

    for(int i = 0; i < totalNodes; i++){
        new (&flatList[i]) LinearBVHNode;
    }

    int offset = 0;
    compactBVH(root, &offset);

    nodeCount = BVHNode::counter;
}

//building our BVH
//totalNodes is a pointer because we want everything pointing to the same totalNodes variable (we DON'T want to pass by value)
BVHNode * BVH::buildBVH(vector<BVHPrimitive> &primList, int start, int end, int *totalNodes, vector<Triangle> &orderedTriangles, int curLevel){
    (*totalNodes)++;

    //create a new BVHNode
    BVHNode *node = new BVHNode(++curLevel);

    //computing bounding box that contains all the primitives in this BVH node
    AABBox bbox;
    for(int i = start; i < end; i++){
        bbox = Union(bbox, primList[i].bounds);
    }

    int numPrim = end - start;


    //if you've got one primitive, create a leaf node
    if(numPrim == 1){
        int firstPrimOffset = orderedTriangles.size();

        for(int i = start; i < end; i++){
            int primNumber = primList[i].primNum;
            orderedTriangles.push_back(triList[primNumber]);
        }

        node->CreateLeaf(firstPrimOffset, numPrim, bbox);

        //log << "I've built a leaf at level: " << node->level << " and the num of tris at this level is: " << numPrim << endl;
    }

    //else create the children by a recursive call to buildBVH
    else{

        /////////////////////////////////////////////////////////////////////////////////////////////
        //i've first got to compute the centroid bounds so I know which axis to split the triangles//
        /////////////////////////////////////////////////////////////////////////////////////////////
        AABBox centroidBounds;

        for(int i = start; i < end; i++){
            centroidBounds = Union(centroidBounds, primList[i].centroid);
            //log << "centroid bounds for " << primList[i].primNum << " are: " << primList[i].centroid << endl;
        }



        int splitDim = centroidBounds.MaximumExtent();

        //log << "splitting along: " << splitDim << endl;

        //log << "splitting along: " << splitDim << endl;
        //log << "splitDim pMax: " << centroidBounds.pMax << endl;
        //log << "splitDim pMin: " << centroidBounds.pMin << endl;

        //after I find the split axis I have to partition the primitives according to it and build the two children of the BVH tree
        int mid = (start + end) / 2;

        //if the centroid points are at the same position, then recursion stops and a leaf node is created
        if(centroidBounds.pMax[splitDim] == centroidBounds.pMin[splitDim]){
            int firstPrimOffset = orderedTriangles.size();

            for(int i = start; i < end; i++){
                int primNumber = primList[i].primNum;
                orderedTriangles.push_back(triList[primNumber]);
            }

            node->CreateLeaf(firstPrimOffset, numPrim, bbox);

            //log << endl;
            //log << "I've built a leaf at level: " << node->level << " and the num of tris at this level is: " << numPrim << endl;
            //log << "This is because my centroid bounds are at the same position" << endl;
            return node;
        }



        /////////////////////////////////////////////////////////////////////////
        //now i'll use Surface area heuristics to split the primitives properly//
        /////////////////////////////////////////////////////////////////////////

        //if I only have a handful of primitives to deal with, just split shit in half
        //s.t. half the smallest centroids are one side and half the bigger ones are on the other
        //I do this because SAH at this point isn't worth it
        if(numPrim <= 10){
            mid = (start + end) / 2;

            nth_element(&primList[start], &primList[mid], &primList[end-1] + 1, ComparePoints(splitDim));
        }

        //else do SAH
        else{
            //we're only putting primitives into 12 buckets
            const int numBuckets = 10;

            BucketInfo buckets[numBuckets];

            //now determine which bucket each primitive IN THE RANGE belongs to and update the bucket's bounds to include that primitive's bounds
            //WE'RE NOT PUTTING SHIT IN BUCKETS YET
            for(int i = start; i < end; i++){
                int bucketDecision = numBuckets * ( (primList[i].centroid[splitDim] - centroidBounds.pMin[splitDim]) /
                                                    (centroidBounds.pMax[splitDim] - centroidBounds.pMin[splitDim]));
                //if we're at the outer border, go back one bucket
                if(bucketDecision == numBuckets){
                    bucketDecision = numBuckets - 1;
                }

                buckets[bucketDecision].count++;
                buckets[bucketDecision].bounds = Union(buckets[bucketDecision].bounds, primList[i].bounds);
            }

            //the intersection time is 1 and the traversal cost to 1/8
            //with this information we compute the cost of splitting after each bucket (in order to pick our split point)
            float cost[numBuckets - 1];
            //numBuckets - 1 because splitting after the last bucket wouldn't split the primitives
            for(int i = 0; i < numBuckets - 1; i++){
                AABBox box0, box1;
                int count0 = 0, count1 = 0;

                //first split
                for(int j = 0; j <= i; j++){
                    box0 = Union(box0, buckets[j].bounds);
                    count0 += buckets[j].count;
                }

                //second split
                for(int j = i + 1; j < numBuckets; j++){
                    box1 = Union(box1, buckets[j].bounds);
                    count1 += buckets[j].count;
                }
                //log << "prob A: " << (box0.SurfaceArea() / bbox.SurfaceArea()) << endl;
                //log << "prob B: " << (box1.SurfaceArea() / bbox.SurfaceArea()) << endl;
                //log << "left count: " << count0 << endl;
                //log << "right count: " << count1 << endl;
                cost[i] = 0.125f + (count0 * box0.SurfaceArea() + count1 * box1.SurfaceArea()) / bbox.SurfaceArea();
            }

            //now that we've got all the costs...find the bucket to split at
            float minCost = cost[0];
            int minCostSplit = 0;
            for(int i = 1; i < numBuckets - 1; i++){
                if(cost[i] < minCost){
                    minCost = cost[i];
                    minCostSplit = i;
                }
            }

            //log << "bucket I picked: " << minCostSplit << endl;

            //now that i've found the bucket I want to split at see if it's worth it to create two children or a leaf node at the split
            //numPrim > 1 checks if I have more than one primitive to see whether the split is worth it
            if(numPrim > 1 || minCost < numPrim){
                BVHPrimitive *primMid = partition(&primList[start], &primList[end - 1] + 1,
                                                  CompareToBucket(minCostSplit, numBuckets, splitDim, centroidBounds));

                //subtracting two pointers gives how many elements there are between the two pointers
                mid = primMid - &primList[0];
            }
            //else create leaf node
            else{
                int firstPrimOffset = orderedTriangles.size();

                for(int i = start; i < end; i++){
                    int primNumber = primList[i].primNum;
                    orderedTriangles.push_back(triList[primNumber]);
                }

                node->CreateLeaf(firstPrimOffset, numPrim, bbox);

                //log << endl;
                //log << "I've built a leaf at level: " << node->level << " and the num of tris at this level is: " << numPrim << endl;
                //log << "This is because it's better to create a leaf than it is to split...cost wise" << endl;
            }

        }

        //log << "difference: " << (mid - start) << endl;

        //creating the interior node and passing in its two children that will get created with the recursive call to buildBVH
        BVHNode *leftChild = buildBVH(primList, start, mid, totalNodes, orderedTriangles, node->level);
        BVHNode *rightChild = buildBVH(primList, mid, end, totalNodes, orderedTriangles, node->level);
        node->CreateInterior(splitDim,
                            leftChild,
                            rightChild);
    }

    return node;
}


//offset is a pointer for the same reason as totalNodes is a pointer
int BVH::compactBVH(BVHNode * node, int *offset){
    LinearBVHNode *linearNode = &flatList[*offset];
    linearNode->bounds = node->bounds;

    //remember...first set, then increment
    int myOffset = (*offset)++;
    //log << "myOffset: " << myOffset << endl;

    //if we have a leaf node
    if(node->numPrim > 0){
        linearNode->primitivesOffset = node->firstPrimOffset;
        //log << "primitives offset: " << linearNode->primitivesOffset << endl;
        linearNode->numPrimitives = node->numPrim;
    }

    //else we have an interior node
    else{
        linearNode->axis = node->splitAxis;
        linearNode->numPrimitives = 0;
        compactBVH(node->children[0], offset);
        linearNode->secondChildOffset = compactBVH(node->children[1], offset);
    }

    return myOffset;
}

BVH::~BVH(){
    if(copyCount.unique()){
        log << "I'm deleting the BVH" << endl;
        delete root;
        delete [] flatList;
    }
}



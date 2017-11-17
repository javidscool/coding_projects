#ifndef VIRTUOSO_MESH_H
#define VIRTUOSO_MESH_H

#include "AttributeArray.h"


//class to construct, read, and write mesh data
//
class Mesh
{

    se::core::CopyCounter copycount;


public:

    enum PrimitiveMode {TRIANGLES=0, TRIANGLE_STRIPS, TRIANGLE_FANS, LINES, LINE_LOOP, POLYLINE, POINTS};

    static unsigned int pointsperface(PrimitiveMode m);

    bool indexedmesh;
    int numFaces;
    int numVerts;

    unsigned int* indexbuffer;

    std::vector<AttributeArray> attributes;
    PrimitiveMode mode;


    void read(std::istream& in);


    void write(std::ostream& out);

    Mesh():indexedmesh(true), numFaces(0), numVerts(0), indexbuffer(NULL), mode(TRIANGLES){

    }

    Mesh(std::istream& in):indexbuffer(NULL){
        read(in);
    }

    virtual ~Mesh();

};



#include "Mesh.tcc"
#endif

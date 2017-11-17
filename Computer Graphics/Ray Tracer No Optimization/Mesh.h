#ifndef VIRTUOSO_MESH_H
#define VIRTUOSO_MESH_H

#include "AttributeArray.h"
#include <Eigen/Geometry>

struct Vertex
{
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    Eigen::Vector2f textCoord;
};


struct Triangle
{
    //our triangle is composed of three vertices
    Vertex vertices[3];

    //constructor that sets our triangle to zeroes
    Triangle(){
        for(int i = 0; i < 3; i++)
        {
            vertices[i].position = Eigen::Vector3f::Zero(3);
            vertices[i].normal = Eigen::Vector3f::Zero(3);
            vertices[i].textCoord = Eigen::Vector2f::Zero(2);
        }
    }


    //doing a deep copy of another triangle
    void Equals(Triangle tri){
        for(int i = 0; i < 3; i++)
        {
            vertices[i].position = tri.vertices[i].position;
            vertices[i].normal = tri.vertices[i].normal;
            vertices[i].textCoord = tri.vertices[i].textCoord;
        }
    }

};


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

    Mesh():indexedmesh(true), numFaces(0), numVerts(0), indexbuffer(NULL), mode(TRIANGLES)
    {

    }

    Mesh(std::istream& in):indexbuffer(NULL)
    {
        read(in);
    }


    std::vector<Triangle> toTriangleList();


    virtual ~Mesh();

};



#include "Mesh.tcc"
#endif

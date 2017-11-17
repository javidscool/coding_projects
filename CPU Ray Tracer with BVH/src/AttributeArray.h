#ifndef VIRTUOSO_ATTRIBUTEARRAY_MESH_H
#define VIRTUOSO_ATTRIBUTEARRAY_MESH_H

//#include <assimp/assimp.h>
//#include <assimp/assimp.hpp>
//#include <assimp/aiTypes.h>
//#include <assimp/aiPostProcess.h>
//#include <assimp/aiScene.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include <se/core/CopyCounter.h>
#include <sstream>

#include <se/graphics/gl/Exceptions.h>
#include <se/core/Log.h>


class AttributeArray
{
    se::core::CopyCounter copycount;

public:

    float* array;
    std::string name;
    unsigned int vertices;
    unsigned int components;

    //constructor to construct an attribute array.  does not allocate any memory yet
    AttributeArray(std::string n,  int vert,  int comps);

    AttributeArray(int vert, std::istream& i);

    void read(std::istream& in);

    void write(std::ostream& out);

    virtual ~AttributeArray();

};


#include "AttributeArray.tcc"
#endif

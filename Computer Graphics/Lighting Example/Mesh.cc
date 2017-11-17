#include "Mesh.h"

//god damn typing
#define log se::core::Log::status()


unsigned int Mesh::pointsperface(PrimitiveMode m)
{

    switch(m) {

        //todo: depending on implementation, special kinds of triangles and lines might not be so good for this
    case TRIANGLES:
        return 3;
        break;
    case TRIANGLE_STRIPS:
        return 3;
        break;
    case TRIANGLE_FANS:
        return 3;
        break;
    case LINES:
        return 2;
        break;
    case LINE_LOOP:
        return 2;
        break;
    case POINTS:
        return 1;
        break;
    default:
        return 0;
        break;
    }

}





void Mesh::read(std::istream& in)
{
//write out basic information

    unsigned int numAttributes = 0;



    in.read(( char*)&numVerts, sizeof(int)); //how many vertices?
    in.read(( char*)&numFaces, sizeof(int)); //how many faces?
    in.read((char*)&mode, sizeof(int)); //output the primitive type
    in.read((char*)&indexedmesh, sizeof(bool));
    in.read((char*)&numAttributes, sizeof(unsigned int));


    log<<"Loading in mesh:"<<std::endl;
    log<<"Vertices:"<<numVerts<<std::endl;
    log<<"Faces:"<<numFaces<<std::endl;
    log<<"Is indexed:"<<indexedmesh<<std::endl;
    log<<"Points per face:"<<pointsperface(mode)<<std::endl;
    log<<"attribute count:"<<numAttributes<<std::endl;

    if(indexbuffer)delete []indexbuffer;
    indexbuffer = new unsigned int[numFaces * pointsperface(mode)];

    if(indexedmesh) {
        in.read(( char*)indexbuffer, sizeof(unsigned int)*numFaces*pointsperface(mode));
    }

    for(unsigned int ppsp = 0; ppsp < numAttributes; ppsp++) {
        attributes.push_back(AttributeArray(numVerts, in));

    }

}


void Mesh::write(std::ostream& out)
{

    //write out basic information
    out.write(( char*)&numVerts, sizeof(int)); //how many vertices?
    out.write(( char*)&numFaces, sizeof(int)); //how many faces?
    out.write((char*)&mode, sizeof(int)); //output the primitive type
    out.write((char*)&indexedmesh, sizeof(bool));
    unsigned int numattribs = attributes.size();
    out.write((char*)&(numattribs), sizeof(unsigned int));

    if(indexedmesh) {
        out.write((const char*)indexbuffer, sizeof(unsigned int)*numFaces*pointsperface(mode));
    }

    for(unsigned int i = 0; i < attributes.size(); i++) {
        attributes[i].write(out);
    }

}


Mesh::~Mesh()
{
    if(copycount.unique() && indexbuffer) {
        delete indexbuffer;
    }
}


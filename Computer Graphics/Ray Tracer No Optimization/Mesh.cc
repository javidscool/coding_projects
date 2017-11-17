#include "Mesh.h"

//god damn typing
#define log se::core::Log::status()


std::vector<Triangle> Mesh::toTriangleList()
{

    int attnormalindex=-1;
    int attpositionindex=-1;
    int attuvindex=-1;


    for(int i = 0; i < attributes.size(); i++ ) {
        log<<attributes[i].name<<std::endl;
        if(attributes[i].name == "position") {
            log<<"setting pos";
            attpositionindex = i;
        }

        else if(attributes[i].name == "texcoords0") {
              log<<"setting tex";
            attuvindex = i;
        }

        else if(attributes[i].name == "normal") {
              log<<"setting norm";
            attnormalindex = i;
        }

    }

    if(attnormalindex <0 || attpositionindex < 0 || attuvindex < 0) {
        throw se::core::Exception("Missing attribute data for raytracer!");
    }

    std::vector<Triangle> tlist;

    //create the actual triangle list
    for(int j = 0; j < numFaces*3; j+=3) { //iterate through the index buffer

        Triangle face;

        for(int kk = 0; kk < 3; kk++) { //initialize each vertex of the triangle

            int indexp0 = indexbuffer[j+kk]; //which vertex is the kk'th vertex of the triangle

            int normalbasep0 = indexp0*3;
            int positionbasep0 = indexp0*3;
            int uvbasep0 =indexp0*2;

            face.vertices[kk].position[0] = attributes[attpositionindex].array[positionbasep0];
            face.vertices[kk].position[1] = attributes[attpositionindex].array[positionbasep0+1];
            face.vertices[kk].position[2] = attributes[attpositionindex].array[positionbasep0+2];

            face.vertices[kk].normal[0] = attributes[attnormalindex].array[normalbasep0];
            face.vertices[kk].normal[1] = attributes[attnormalindex].array[normalbasep0+1];
            face.vertices[kk].normal[2] = attributes[attnormalindex].array[normalbasep0+2];

            face.vertices[kk].textCoord[0] = attributes[attuvindex].array[uvbasep0];
            face.vertices[kk].textCoord[1] = attributes[attuvindex].array[uvbasep0+1];
        }


        tlist.push_back(face);

    }


if(tlist.size() != numFaces){
throw se::core::Exception("didn't hit all faces!");
}else{
log<<"got all the triangles"<<std::endl;
}
    return tlist;
}



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


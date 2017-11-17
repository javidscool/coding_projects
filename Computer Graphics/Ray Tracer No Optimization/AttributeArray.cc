#include "AttributeArray.h"

//god damn typing
#define log se::core::Log::status()

//constructor to construct an attribute array.  does not allocate any memory yet
AttributeArray::AttributeArray(std::string n,  int vert,  int comps):array(NULL), name(n), vertices(vert), components(comps)
{
    array = new float[vertices*components];
}

AttributeArray::AttributeArray(  int vert, std::istream& i):name(""), vertices(vert)
{

    read(i);
}




void AttributeArray::read(std::istream& in)
{

    unsigned int namelength;

    in.read((char*) &namelength, sizeof(unsigned int));
    char* c_str = new char[namelength];

    in.read((char*)c_str, sizeof(char)*namelength);

    name = std::string(c_str);

    in.read((char*)&components, sizeof(unsigned int));

    array = new float[components * vertices];

    in.read((char*) array, vertices*components*sizeof(float));

    log<<"loading attribute:"<<name<<" with "<<components<<" components per vertex"<<std::endl;

    delete []c_str;

}


void AttributeArray::write(std::ostream& out)
{
    unsigned int namelength = name.size()+1;
    out.write((char*)&namelength, sizeof(unsigned int));
    out.write((const char*)name.c_str(), sizeof(char)*(namelength));
    out.write((const char*)&components, sizeof(unsigned int));
    out.write((char*)array, vertices*components*sizeof(float));
}


AttributeArray::~AttributeArray()
{
    if(copycount.unique()&&array) {

        delete []array;
    }

}

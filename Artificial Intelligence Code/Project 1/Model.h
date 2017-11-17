#ifndef MODEL
#define MODEL

#include <irrlicht.h>

using namespace irr;

class Model{

public:
	scene::IAnimatedMesh* mesh;
	video::ITexture* texture;
	double scale;

	Model(){};
	
	~Model(){
		//mesh->drop();
		//texture->drop();
	};
	
	Model(const char* m, const char* s, IrrlichtDevice *d, double sc=1.0f);
	
	//CHUCKIE (createModel("../media/chuckie.MD2","../media/Chuckie.pcx",device))
};

Model createModel(const char* m, const char* s, IrrlichtDevice *d, double sc=1.0f);

#endif
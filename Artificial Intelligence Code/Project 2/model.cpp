#include "Model.h"
#include <string>

//creates a 3d model data structure
Model createModel(const char* m, const char* s, IrrlichtDevice *d, double sc){
	Model mod;
	mod.mesh = d->getSceneManager()->getMesh(m);
	mod.texture = d->getVideoDriver()->getTexture(s);
	mod.scale = sc;
	if(!(mod.mesh&&mod.texture))
		exit(0);
	
	return mod;
}



Model::Model(const char* m, const char* s, IrrlichtDevice *d, double sc){
	//Model mod;
	mesh = d->getSceneManager()->getMesh(m);
	texture = d->getVideoDriver()->getTexture(s);
	scale = sc;
	if(!(mesh&&texture)){
		throw new std::string("Error creating model");
	}
	//return mod;
}

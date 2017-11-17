#ifndef CANENTITY
#define CANENTITY
#include "irrlicht.h"
#include "GameEntity.h"
#include "physicsObject.h"

#define cannode mynodep

using namespace irr;
using namespace irr::core;

class canEntity : public physicsObject{

public:
	
	//irr::scene::ISceneNode* cannode;
	canEntity(IrrlichtDevice *device);
	virtual void update(const irr::ITimer* timer);
	virtual bool processMessage(const Message*);
	//canEntity(){};

};

#endif
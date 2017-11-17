#ifndef COVEROBJECT
#define COVEROBJECT

#include "irrlicht.h"
#include "physicsObject.h"

class coverObject : public physicsObject{

private:
	irr::scene::ISceneNode* spot;

public:
	
	irr::core::vector3df getCoverPosition(physicsObject* hidee){
		
		//was const, caused bug, todo: find out why
		irr::core::vector3df center = mynodep->getPosition();
		irr::core::vector3df tVec = (center - hidee->getSceneNode()->getPosition());//.normalize();
		tVec.Y = 0;
		tVec = tVec.normalize();
	
		//tVec = vector3df(0,0,0);
		const double hideRadius = 10.0f;
		double objectRadius = 71.0f/2; //by pythagoras, using the diagonal of the 50x50 square as the radius 

		spot->setPosition((tVec * (hideRadius + objectRadius)) + this->mynodep->getPosition());
		//spot->setMaterialFlag(video::EMF_ZBUFFER, false);
		spot->setVisible(true);
		return (tVec * (hideRadius + objectRadius)) + this->mynodep->getPosition();

	}
	
	static double getRadius(){return 50;}
	static double getBoundaryRadius(){return getRadius() + 10;}



	coverObject(irr::core::vector3df p, irr::IrrlichtDevice* device){
		
		mynodep = device->getSceneManager()->addCubeSceneNode(1);
		mynodep->setPosition(p);
		mynodep->setScale(vector3df(50,75,50));
		mynodep->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/crate.jpg"));
		mynodep->setMaterialTexture(1, device->getVideoDriver()->getTexture("../media/cratebump.jpg"));
		mynodep->setMaterialFlag(video::EMF_LIGHTING, true);
		mynodep->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		mynodep->setMaterialType(video::EMT_LIGHTMAP_LIGHTING_M4);

	//	mynodep->setVisible(false);
		spot = device->getSceneManager()->addSphereSceneNode(10);
		//spot->setMaterialFlag(video::EMF_ZBUFFER, false);
		spot->setPosition(mynodep->getPosition());
		spot->setVisible(true);
		//mynodep->addChild(spot);
		
		setPosition(p);
	}

	~coverObject(){
		if(mynodep){
			mynodep->drop();
		}
	}
	
	virtual bool processMessage(const Message*){return false;}
	virtual void update(const irr::ITimer* t){;}

};

#endif
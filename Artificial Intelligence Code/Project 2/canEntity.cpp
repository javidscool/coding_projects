#include "canEntity.h"
#include "Message.h"
using namespace irr;
using namespace irr::core;
#include <iostream>

canEntity::canEntity(IrrlichtDevice *device){


	killCount = 0;
	
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	video::IVideoDriver* driver = device->getVideoDriver();
	
	cannode = smgr->addSphereSceneNode(25,128);
	//cannode->setScale(irr::core::vector3df(5,5,5));

	//CPTODO::replace constant with something better
	cannode->setPosition(irr::core::vector3df(107,30,93));
	this->setPosition(cannode->getPosition());
	cannode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	cannode->setMaterialTexture(0,driver->getTexture("../media/skydome.jpg"));
	cannode->setMaterialType(irr::video::EMT_SPHERE_MAP);
	
	irr::scene::ILightSceneNode *lightscenenode = smgr->addLightSceneNode(0,  irr::core::vector3df(25,25,25), irr::video::SColor(255, 255, 0, 0),500);
	irr::scene::ILightSceneNode *lightscenenode2 = smgr->addLightSceneNode(0, irr::core::vector3df(-25,-25,-25), irr::video::SColor(255, 255, 0, 0),500);
	cannode->addChild(lightscenenode);
	cannode->addChild(lightscenenode2);


	const double startAngle = 0;
	const double endAngle = 2*3.14159;
	const double nodeCount =12.0f;
	const double radius = 45.0f;
	double increment = (startAngle - endAngle) / nodeCount;
	
	for(int i = 0; i < nodeCount; i++){
		double currentAngle = endAngle + increment*i;
		irr::scene::ISceneNode* a = smgr->addSphereSceneNode(5,16);
		a->setPosition(radius*vector3df( cos(currentAngle) ,sin(currentAngle),0 ));
		this->getSceneNode()->addChild(a);
		a->setMaterialTexture(0,driver->getTexture("../media/skydome.jpg"));
		a->setMaterialType(video::EMT_SPHERE_MAP);
	}


	for(int i = 0; i < nodeCount; i++){
		double currentAngle = endAngle + increment*i;
		irr::scene::ISceneNode* a = smgr->addSphereSceneNode(3,16);
		a->setPosition(30*vector3df( 0,cos(currentAngle) ,sin(currentAngle) ));
		this->getSceneNode()->addChild(a);
		a->setMaterialTexture(0,driver->getTexture("../media/skydome.jpg"));
		//a->setMaterialType(video::EMT_SPHERE_MAP);
	}
}


void canEntity::update(const irr::ITimer* timer){
	
	static double LASTUPDATE = timer->getTime();
	double timeelapsed = timer->getTime() - LASTUPDATE;

	this->velocity = (cannode->getPosition() - this->position) / timeelapsed;

	//std::cout<<"SPEED"<<this->velocity.getLength()<<std::endl;
	this->setPosition(cannode->getPosition());
	
	cannode->setPosition(vector3df(107.0f,30.0f,93.0f) + 2.0f*(float)sin((double)timer->getTime()/500)*vector3df(0,1,0));

	LASTUPDATE = timer->getTime();

	mynodep->setRotation(vector3df(0,1,0)*timer->getTime()/3);
}


bool canEntity::processMessage(const Message* m){

	switch(m->messageType){
	
		case KTC_KILL: killCount++;break;
		case KTC_REVIVE: killCount = 0;break;

		default:break;
	
	}

	return true;
}
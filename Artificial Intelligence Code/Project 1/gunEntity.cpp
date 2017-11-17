#include "gunEntity.h"
#include <iostream>
using namespace irr;
using namespace irr::core;

static vector3df previousPos;

gunEntity::gunEntity(irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *camera){		animationReady = true;	this->device = device;
	//set time gun was last fired	lastFired = device->getTimer()->getTime();		irr::scene::ISceneManager* smgr = device->getSceneManager();	Model GUN = createModel("../media/dreadus_shotgun.md2","../media/shotgun_map.png",device,1.0f);		gun = smgr->addAnimatedMeshSceneNode(GUN.mesh);		gun->setRotation(vector3df(0,270,0));	gun->setPosition(				(vector3df(.0f,.0f,.0f)				//+camera->getTarget()				)//.normalize()			);
	gun->setMD2Animation(irr::scene::EMAT_STAND);	gun->setAnimationSpeed(60);	gun->setMaterialTexture(0,GUN.texture);	gun->setMaterialFlag(video::EMF_FOG_ENABLE, true);	gun->setLoopMode(false);
	gun->setPosition(vector3df(0,-1000,0));	previousPos = camera->getPosition();	this->camera = camera;	basePosition = gun->getPosition();}void gunEntity::update(const irr::ITimer* timer){
	vector3df t = (camera->getPosition() - previousPos);
	t.Y = 0;
	if(t.getLength() > .05f){
		gun->setPosition(basePosition + 1.0f*(float)sin((double)timer->getTime()/125)*vector3df(.25,1,0));
	}
	else{
		gun->setPosition(basePosition);
	}
	
	previousPos = camera->getPosition();

	//Takes the delta of time between when the gun was last fired and current time	//to determine what the barValue should be	irr::u32 delta = device->getTimer()->getTime() - lastFired;	int barValue = 0 ;	if(delta >= 10000)// if its been longer than 10 seconds	{		gameHUD::getInstance()->setGunReady(true);		gameHUD::getInstance()->setShotTimerBarValue(100);	}	else if(delta < 10000)	{		gameHUD::getInstance()->setGunReady(false);		//update bar value every 1%		barValue = delta/100;		gameHUD::getInstance()->setShotTimerBarValue(barValue);	}		if(gun->getFrameNr() >= gun->getEndFrame()){		animationReady = true;				}
}

bool gunEntity::processMessage(const Message* m){
	//std::cout<<"got to gun processmessage\n";	//if(m == NULL)std::cout<<"WTF THIS IS NULL SOMEHOW\n";		switch(m->messageType){
	
		case KTC_PLAYER_LEFT_MOUSE_CLICK:
			if(gun->getFrameNr() >= gun->getEndFrame()){
				//animation is not ready
				animationReady = false;
				//Set timer bar to zero
				gameHUD::getInstance()->setShotTimerBarValue(0);
				//set time last fired
				lastFired = device->getTimer()->getTime();
				
				gun->setMD2Animation(irr::scene::EMAT_STAND);
				gun->setAnimationSpeed(60);
			}
			//std::cout<<"did stuff\n";\

		break;
		default:;//std::cout<<"Wrong message type\n";

	}

	return true;
}



void gunEntity::render(){
	//std::cout << "I'm in the gun renderer.\n";	device->getVideoDriver()->clearZBuffer();		irr::core::matrix4 abc(irr::core::IdentityMatrix);	const float mdat[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,1000,0,1};	abc.setM(mdat);		device->getVideoDriver()->setTransform(video::ETS_WORLD,camera->getAbsoluteTransformation());// camera->getAbsoluteTransformation());	device->getVideoDriver()->setTransform(video::ETS_VIEW, abc);		if(gun->isVisible())		gun->render();}
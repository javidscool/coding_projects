#include "player.h"

using namespace irr;
using namespace irr::scene;

void player::gunInit(){
	cout << "Got into gun init.\n";
	if(type == PREY){
		cout << "I'm prey and I don't have a gun.\n";
		player::getGun().getSceneNode()->setVisible(false);
	}
	else if(type == PREDATOR){
		cout  << "I'm a predator and I have a gun.\n";
		player::getGun().getSceneNode()->setVisible(true);
	}
}

void player::setSpeed(){
	player::gunInit();
	core::list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin(); 
	ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
	if(type == PREY){
		MAXSPEED = .3f;
		anim->setMoveSpeed(MAXSPEED);
	}
	else{
		MAXSPEED = .15f;
		anim->setMoveSpeed(MAXSPEED);
	}

}

void player::useSpectatorCamera(){
	core::list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin(); 
	ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims; 
	
	//anim->setVerticalMovement(true);
	throw std::string("Not done yet");
}

void player::useShooterCamera(){
	core::list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin(); 
	ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims; 
	anim->setVerticalMovement(false);
}
	
void player::setPosition(irr::core::vector3df n){
	mynodep->setPosition(n);
	this->position = n;
	ppos = n;
}

	
player::player(irr::IrrlichtDevice* dev, irr::core::vector3df sp, Timer tim, Timer inv, GamePlayer_Type T) : device(dev), camera (dev->getSceneManager()->addCameraSceneNodeFPS()) , gun (dev, camera), GamePlayer(sp, tim, inv, T){
	ppos = camera->getPosition();
	this->mynodep = camera;
	lastUpdate = 0;
	scene::ISceneNode *lightscenenode4 = dev->getSceneManager()->addLightSceneNode(0, vector3df(0,0,0), irr::video::SColor(255, 100, 100, 0),200);
	camera->addChild(lightscenenode4);
	useShooterCamera();

	//set up state machine
	PlayerStateMachine = new StateMachine<player>(*this);
	
	if(type == PREDATOR){
		cout << "player is a predator.\n";
		//starting at wait state
		PlayerStateMachine->SetCurrentState( PlayerWait::GetInstance() );
		PlayerStateMachine->StartCurrentState();
		cout << "I've set the current state.\n";
	}
	else{
		cout << "player is prey.\n";
		//starting at play state
		PlayerStateMachine->SetCurrentState( PlayerPlay::GetInstance() );
		PlayerStateMachine->StartCurrentState();
		cout << "I've set the current state.\n";
	}
}

player::~player(){
	//deleting the state machine
	delete PlayerStateMachine;
}


void player::update(const irr::ITimer* timer){
	
	PlayerStateMachine->update(timer);
	
	double timeElapsed = timer->getTime()- lastUpdate;
	lastUpdate = timer->getTime();
	velocity = mynodep->getPosition() - ppos / timeElapsed;
	this->setPosition(mynodep->getPosition());
	ppos = mynodep->getPosition();
	gun.update(timer);

	pl_time.update(timer);

	pl_inv.update(timer);

	if(pl_inv.getTime() >= 0){
		//cout << "I'm invincible now, yay!\n";
	}
}


bool player::processMessage(const Message* msg){
	return PlayerStateMachine->processMessage(msg);
}




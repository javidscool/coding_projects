#ifndef PLAYER
#define PLAYER
#include "irrlicht.h"
#include "gunEntity.h"
#include "GamePlayer.h"
#include "StateMachine.h"
#include "PlayerStates.h"
#include "Timer.h"
#include <string>

using namespace irr;
using namespace irr::core;
using namespace irr::scene;


class player : public GamePlayer{

private:
	//Camera Scene Node
	irr::IrrlichtDevice *device;	scene::ICameraSceneNode *camera;
	
	gunEntity gun;
	vector3df ppos; 
	double lastUpdate;

	StateMachine<player> * PlayerStateMachine;

public:
	virtual void setSpeed();
	void gunInit();
	void useSpectatorCamera();	void useShooterCamera();	
	irr::IrrlichtDevice * getDevice(){ return device; }		virtual void setPosition(irr::core::vector3df n);
	
	player(irr::IrrlichtDevice* dev, irr::core::vector3df sp, Timer tim, Timer inv, GamePlayer_Type T);
	
	player(){}
	virtual ~player();

	
	//fsm getter
	inline StateMachine<player> * GetFSM() const{ return PlayerStateMachine; }

	virtual void update(const irr::ITimer* timer);

	virtual bool processMessage(const Message* msg);

	inline gunEntity& getGun(){return this->gun;}

};


#endif
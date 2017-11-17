#ifndef GUNENTITY
#define GUNENTITY
#include "GameEntity.h"
#include "gameHUD.h"
#include "Model.h"
#include "Message.h"

class gunEntity : public GameEntity{

private:
	irr::u32 lastFired;

public:
	irr::scene::IAnimatedMeshSceneNode* gun;
	irr::scene::ICameraSceneNode *camera;
	irr::core::vector3df basePosition;
	irr::IrrlichtDevice *device;
	irrklang::ISoundEngine *sound;
	irrklang::ISoundSource* gunFiring;
	bool animationReady;

	inline irr::scene::IAnimatedMeshSceneNode* getSceneNode(){ return gun; }
	//gunEntity(){};
	//gunEntity(irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *camera);
	gunEntity(irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *camera, irrklang::ISoundEngine *soundEngine);
	virtual void update(const irr::ITimer*);
	virtual bool processMessage(const Message*);
	void render();
	bool isReady(){return animationReady;}

	gunEntity(){}

};

#endif
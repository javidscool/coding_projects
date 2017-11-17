#ifndef GAMEENTITY
#define GAMEENTITY

#include <irrlicht.h>

#include <vector>

class Message;
class Agent;
class physicsObject;

class GameEntity{

protected:

public:
	
	virtual bool processMessage(const Message*)=0;
	virtual void update(const irr::ITimer*)=0;
	
	//GameEntity();
	//virtual ~GameEntity();
};

#endif
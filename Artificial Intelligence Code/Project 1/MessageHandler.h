#ifndef MESSAGEHANDLER
#define MESSAGEHANDLER

#include <irrlicht.h>
#include <set>


class GameEntity;
//class Message;

#include "Message.h"

#define MsgHandler MessageHandler::getInstance()

class MessageHandler{

private:
	std::set<Message *> messageQueue;
	
	MessageHandler();
	~MessageHandler();
	void deliverMessage(const Message* m);

public:
	void postMessage(Message_Type type, int delay, GameEntity *sender, GameEntity *receiver,const irr::ITimer* timer);
	
	int update(const irr::ITimer*); 
	
	static MessageHandler *getInstance();		
};

#endif
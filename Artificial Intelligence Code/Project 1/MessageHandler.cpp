#include <irrlicht.h>
#include <set>
#include "MessageHandler.h"
#include "GameEntity.h"

#include <iostream>
using std::cout;
		 
MessageHandler::MessageHandler(){

}
 

MessageHandler::~MessageHandler(){

}



void MessageHandler::postMessage(Message_Type type, int delay, GameEntity *sender, GameEntity *receiver, const irr::ITimer* timer){
	Message* m = new Message(delay + timer->getTime(), type, sender, receiver);
	
	if(delay == 0){
		deliverMessage(m);
	}
	else messageQueue.insert(m);
}
	

int MessageHandler::update(const irr::ITimer* timer){
	

	//if(messageQueue.empty())return 0;
	//working with delayed messages
	while(  !messageQueue.empty() && (*(messageQueue.begin()))->postTime  < (int)timer->getTime()){

		//deliverMessage(&*messageQueue.begin());
	    //deliverMessage(const_cast<const Message*>(& (*messageQueue.begin())) );
		
		const Message* a = (*(messageQueue.begin()));
		deliverMessage(a);
		messageQueue.erase(messageQueue.begin());
	}

	return 0;
}



void MessageHandler::deliverMessage(const Message* m){

	if(m->receiver->processMessage(m))
		cout << "Message was handled.\n";

	else cout<< "uh oh, message not handled\n";
	delete m;
}

 MessageHandler* MessageHandler::getInstance(){
	 
	 static MessageHandler only_inst;
	 
	 return &only_inst;
}

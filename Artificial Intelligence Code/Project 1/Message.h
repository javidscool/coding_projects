class GameEntity;

#ifndef MESSAGE
#define MESSAGE

#include "irrlicht.h"

enum Message_Type {
	KTC_ACTIVATE, KTC_KILL, KTC_REVIVE,	KTC_SPOTTED,
	KTC_ENDROUND, KTC_BEGIN_ROUND,KTC_START_GAME,
	KTC_PAUSE_GAME,KTC_RESUME_GAME,KTC_END_GAME,
	KTC_PLAYER_LEFT_MOUSE_CLICK,KTC_PLAYER_RIGHT_MOUSE_CLICK,
	APP_INIT, APP_START_GAME, APP_CONFIG_GAME, APP_EXIT_APP,
	APP_MAIN_MENU
};

class Message{	

public:
	
	int postTime;
	Message_Type messageType;
	GameEntity *sender;
	GameEntity *receiver;
	Message(){}
	Message(int, Message_Type, GameEntity *, GameEntity *);
	bool operator==(Message) const;
	bool operator<(Message) const;
	bool operator>(Message) const ;
	bool operator>=(Message) const;
	bool operator<=(Message) const;  
 };

#endif
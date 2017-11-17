#ifndef PLAYER_STATES_H_
#define PLAYER_STATES_H_

#include "State.h"

class player;

//Play state
class PlayerPlay : public State<player>{

	private:
		PlayerPlay(){}
		

	public:
		
		static PlayerPlay* GetInstance();
		
		virtual void Enter(player & plyr);
		
		virtual void Execute(player & plyr, const irr::ITimer*);
		
		virtual void Exit(player & plyr);

		virtual bool ExecuteMessage(player & plyr, const Message *);
};


//Wait state
class PlayerWait : public State<player>{

	private:
		int start;
		int finish;

		PlayerWait(){}


	public:
		
		static PlayerWait* GetInstance();
		
		virtual void Enter(player & plyr);
		
		virtual void Execute(player & plyr, const irr::ITimer* timer);
		
		virtual void Exit(player & plyr);

		virtual bool ExecuteMessage(player & plyr, const Message *msg);
};

#endif

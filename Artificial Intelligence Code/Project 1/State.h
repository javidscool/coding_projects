#ifndef STATE_H_
#define STATE_H_

#include "Message.h"

template <class Game_Entity_Type>
class State{

	public:
		virtual ~State(){}
		
		//this will execute when the state is entered
		virtual void Enter(Game_Entity_Type &) = 0;

		//this is the states normal update function
		virtual void Execute(Game_Entity_Type &, const irr::ITimer*) = 0;

		//this will execute when the state is exited
		virtual void Exit(Game_Entity_Type &) = 0;
		
		//this executes if the agent receives a message from the message dispatcher
		virtual bool ExecuteMessage(Game_Entity_Type &, const Message *) = 0;

};

#endif
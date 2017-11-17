#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <cassert>
#include <string>
#include <iostream>

#include "State.h"


template <class Game_Entity_Type>
class StateMachine{

	private:
		//a pointer to the agent that owns this instance
		Game_Entity_Type & agt_owner;

		State<Game_Entity_Type>* cur_state;
  
		//a record of the last state the agent was in
		State<Game_Entity_Type>* prev_state;

		//this is called every time the FSM is updated
		State<Game_Entity_Type>* glob_state;
  

	public:

		StateMachine(Game_Entity_Type & owner):agt_owner(owner), cur_state(0), prev_state(0), glob_state(0) {}

		virtual ~StateMachine(){}

		//use these methods to initialize the FSM
		void SetCurrentState(State<Game_Entity_Type>* c){cur_state = c;}
		void SetGlobalState(State<Game_Entity_Type>* g) {glob_state = g;}
		void SetPreviousState(State<Game_Entity_Type>* p){prev_state = p;}
  
		void StartCurrentState(){
			//start the entry method of the new state
			cur_state->Enter(agt_owner);
		}

		void StartGlobalState(){
			//start the entry method of the new state
			glob_state->Enter(agt_owner);
		}

		void StartPrevState(){
			//start the entry method of the new state
			prev_state->Enter(agt_owner);
		}

		//call this to update the FSM
		void update(const irr::ITimer* timer)const{
			
			//if a global state exists, call its execute method, else do nothing
			if(glob_state){
				//std::cout << "I'm in glob-state update.\n";
				glob_state->Execute(agt_owner, timer);
			}
			
			//same for the current state
			if (cur_state){
				//std::cout << "I'm in cur-state update.\n";
				cur_state->Execute(agt_owner, timer);
			}
		}

		//change to a new state
		void ChangeState(State<Game_Entity_Type>* NewState){
			
			//if you try to initialize to a NULL state, terminate the program
			if(NewState == NULL){
				std::cout << "Can't initialize the current state to NULL.\n";
				assert(NewState);
			}

			//keep a record of the previous state
			prev_state = cur_state;
			
			//call the exit method of the existing state
			cur_state->Exit(agt_owner);

			//change state to the new state
			cur_state = NewState;

			//call the entry method of the new state
			cur_state->Enter(agt_owner);
		}

		//change state back to the previous state
		void RevertToPreviousState(){
			ChangeState(prev_state);
		}

		//returns true if the current state's type is equal to the type of the
		//class passed as a parameter. 
		bool isInState(const State<Game_Entity_Type>& state) const{
			return typeid(*cur_state) == typeid(state);
		}

		bool processMessage(const Message *msg){
			//see if the current state is valid and that it can handle the message
			if(cur_state && cur_state->ExecuteMessage(agt_owner, msg)){
				return true;
			}
			
			//if there was no state that could handle the message, return false
			else{
				return false;
			}
		}
		
		//getters
		inline State<Game_Entity_Type>* CurrentState() const{return cur_state;}
		inline State<Game_Entity_Type>* GlobalState() const{return glob_state;}
		inline State<Game_Entity_Type>* PreviousState() const{return prev_state;}
};

#endif



#ifndef AGENT_STATES_H_
#define AGENT_STATES_H_

#include "State.h"

class Agent;


//Defend state
class Defend : public State<Agent>{

	private:
		Defend(){}
		
		//copy constructor and assignment operator
		//Defend(const Defend &);
        //Defend & operator=(const Defend &);

	public:
		
		static Defend* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer*);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *);
};


//Patrol state
class Patrol : public State<Agent>{

	private:
		Patrol(){}
		
		//copy constructor and assignment operator
		//Patrol(const Patrol &);
        //Patrol & operator=(const Patrol &);

	public:
		
		static Patrol* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer*);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *);
};


//Pursue state
class Pursue : public State<Agent>{

	private:
		Pursue(){}
		
		//copy constructor and assignment operator
		//Pursue(const Pursue &);
        //Pursue & operator=(const Pursue &);

	public:
		
		static Pursue* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer*);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *);
};


//Hide state
class Hide : public State<Agent>{

	private:
		Hide(){}
		
		//copy constructor and assignment operator
		//Hide(const Hide &);
        //Hide & operator=(const Hide &);

	public:
		
		static Hide* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer*);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *);
};


//Flee state
class Flee : public State<Agent>{

	private:
		Flee(){}
		
		//copy constructor and assignment operator
		//Flee(const Flee &);
        //Flee & operator=(const Flee &);

	public:
		
		static Flee* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer*);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *);
};


//Activating Orb state
class Act_Orb : public State<Agent>{

	private:
		Act_Orb(){}
		
		//copy constructor and assignment operator
		//Act_Orb(const Act_Orb &);
        //Act_Orb & operator=(const Act_Orb &);

	public:
		
		static Act_Orb* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer* timer);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *msg);
};

//Die state
class Die : public State<Agent>{

private:
	Die(){}

public:
	static Die* GetInstance();
	
	virtual void Enter(Agent & agt);
	
	virtual void Execute(Agent & agt, const irr::ITimer*);
	
	virtual void Exit(Agent & agt);
	
	virtual bool ExecuteMessage(Agent &, const Message*);
};


//Wait state
class Wait : public State<Agent>{

	private:
		Wait(){}


	public:
		
		static Wait* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer* timer);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *msg);
};

//Start state
class Start : public State<Agent>{

	private:
		int start;
		int finish;

		Start(){}


	public:
		
		static Start* GetInstance();
		
		virtual void Enter(Agent & agt);
		
		virtual void Execute(Agent & agt, const irr::ITimer* timer);
		
		virtual void Exit(Agent & agt);

		virtual bool ExecuteMessage(Agent &, const Message *msg);
};

#endif
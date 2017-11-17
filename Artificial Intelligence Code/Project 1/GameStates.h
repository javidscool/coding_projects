#ifndef GAME_STATES_H_
#define GAME_STATES_H_

#include "State.h"

class ktcGame;

class PrePlay : public State<ktcGame>{
private:PrePlay(){}

public :
	static PrePlay* getInstance();
	virtual void Enter(ktcGame & game);
	virtual void Execute(ktcGame & game, const irr::ITimer*);
	virtual void Exit(ktcGame & game);
	virtual bool ExecuteMessage(ktcGame &, const Message*);
};

class Play : public State<ktcGame>{
private:Play(){}

public :
	static Play* getInstance();
	virtual void Enter(ktcGame & game);
	virtual void Execute(ktcGame & game, const irr::ITimer*);
	virtual void Exit(ktcGame & game);
	virtual bool ExecuteMessage(ktcGame &, const Message*);
};

class Pause : public State<ktcGame>{
private:Pause(){}

public :
	static Pause* getInstance();
	virtual void Enter(ktcGame & game);
	virtual void Execute(ktcGame & game, const irr::ITimer*);
	virtual void Exit(ktcGame & game);
	virtual bool ExecuteMessage(ktcGame &, const Message*);
};

class RoundBreak : public State<ktcGame>{
private:RoundBreak(){}

public :
	static RoundBreak* getInstance();
	virtual void Enter(ktcGame & game);
	virtual void Execute(ktcGame & game, const irr::ITimer*);
	virtual void Exit(ktcGame & game);
	virtual bool ExecuteMessage(ktcGame &, const Message*);
};

class EndersGame : public State<ktcGame>{
private:EndersGame(){}

public :
	static EndersGame* getInstance();
	virtual void Enter(ktcGame & game);
	virtual void Execute(ktcGame & game, const irr::ITimer*);
	virtual void Exit(ktcGame & game);
	virtual bool ExecuteMessage(ktcGame &, const Message*);
};

#endif
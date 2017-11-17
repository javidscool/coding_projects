#ifndef APP_STATES_H_
#define APP_STATES_H_

#include "State.h"

class App;

class Init : public State<App>{
private:Init(){}

public :
	static Init* getInstance();
	virtual void Enter(App & app);
	virtual void Execute(App & app, const irr::ITimer* timer);
	virtual void Exit(App & app);
	virtual bool ExecuteMessage(App &, const Message*);
};

class MainMenu : public State<App>{
private:MainMenu(){}

public :
	static MainMenu* getInstance();
	virtual void Enter(App & app);
	virtual void Execute(App & app, const irr::ITimer* timer);
	virtual void Exit(App & app);
	virtual bool ExecuteMessage(App &, const Message*);
};

class PlayGame : public State<App>{
private:PlayGame(){}

public :
	static PlayGame* getInstance();
	virtual void Enter(App & app);
	virtual void Execute(App & app, const irr::ITimer* timer);
	virtual void Exit(App & app);
	virtual bool ExecuteMessage(App &, const Message*);
};

class ConfigGame : public State<App>{
private:ConfigGame(){}

public :
	static ConfigGame* getInstance();
	virtual void Enter(App & app);
	virtual void Execute(App & app, const irr::ITimer* timer);
	virtual void Exit(App & app);
	virtual bool ExecuteMessage(App &, const Message*);
};

class ExitApp : public State<App>{
private:ExitApp(){}

public :
	static ExitApp* getInstance();
	virtual void Enter(App & app);
	virtual void Execute(App & app, const irr::ITimer* timer);
	virtual void Exit(App & app);
	virtual bool ExecuteMessage(App &, const Message*);
};


#endif
#include "GameStates.h"
#include "ktcGame.h"
#include "MessageHandler.h"

#include <iostream>
using std::cout;

//methods for PrePlay
PrePlay* PrePlay::getInstance(){

  static PrePlay only_inst;

  return &only_inst;
}

void PrePlay::Enter(ktcGame & game){
	cout << "Entering PrePlay state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
	
	//Game now has their own timer
	game.getPreTime()->setTime(10000);
	game.getPreTime()->setLastTime(game.getDevice()->getTimer()->getTime());
	game.getGameHUD()->setGameState(0);

	//re-initialize player scores
	//game.getGameHUD()->setScores(1,0);
	//game.getGameHUD()->setScores(2,1234);
	//game.getGameHUD()->setScores(3,500000);
	//game.getGameHUD()->setScores(4,100000000);
	//game.getGameHUD()->setScores(5,56789);
}

void PrePlay::Execute(ktcGame & game, const irr::ITimer* timer){
	//cout << "Executing PrePlay state.\n";
	//update round timer
	game.getPreTime()->update(timer);

	//update HUD	
	game.getGameHUD()->getInstance()->updateRoundTimer(	
		game.getPreTime()->getMins(),
		game.getPreTime()->getSecsSecond(),
		game.getPreTime()->getSecsFirst());	
	
	//Pre-Play Time is up
	if((timer->getTime() - game.getLastTime()) > 10000)	
	{
		//Set last time for offset
		game.setLastTime(timer->getTime());	
		//Change to Play State		
		game.GetFSM()->ChangeState(Play::getInstance());
	}

	game.update(timer);
}

void PrePlay::Exit(ktcGame & game){
	cout << "Exiting PrePlay state.\n";
}

bool PrePlay::ExecuteMessage(ktcGame & game, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_START_GAME:	cout << "Starting the game.\n";
						return true;
		case KTC_END_GAME:	cout << "Ending the game\n";
							return true;
	}
	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}


//methods for Play
Play* Play::getInstance(){

  static Play only_inst;

  return &only_inst;
}

void Play::Enter(ktcGame & game){
	cout << "Entering Play state.\n";
	//Game now has their own timer
	game.getRoundTime()->setTime(20000);
	game.getRoundTime()->setLastTime(game.getDevice()->getTimer()->getTime());
	game.getGameHUD()->setGameState(1);

	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void Play::Execute(ktcGame & game, const irr::ITimer* timer)
{
	//cout << "Executing Play state.\n";
	//update round timer	
	game.getRoundTime()->update(timer);
	//update HUD	
	game.getGameHUD()->getInstance()->updateRoundTimer(
		game.getRoundTime()->getMins(),
		game.getRoundTime()->getSecsSecond(),
		game.getRoundTime()->getSecsFirst());

	//if time is up, then round robin shit so that we get new predator and prey
	if(game.getRoundTime()->getTime() <= 0){
		game.RoundRobin(game.getPlayerList());
		//Set last time for offset
		game.getRoundTime()->setLastTime(timer->getTime());	
		//Change to Play State		
		game.GetFSM()->ChangeState(RoundBreak::getInstance());
	}

	game.update(timer);
}

void Play::Exit(ktcGame & game){
	cout << "Exiting Play state.\n";
}

bool Play::ExecuteMessage(ktcGame & game, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_PAUSE_GAME:	cout << "Starting the game.\n";
						return true;
		case KTC_END_GAME:	cout << "Ending the game\n";
							return true;
	}
	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for Pause
Pause* Pause::getInstance(){

  static Pause only_inst;

  return &only_inst;
}

void Pause::Enter(ktcGame & game){
	cout << "Entering Pause state.\n";
	game.getGameHUD()->setGameState(2);
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void Pause::Execute(ktcGame & game, const irr::ITimer* timer){
///	cout << "Executing Pause state.\n";

	game.update(timer);
	//put ChangeState shit here in if conditions
}

void Pause::Exit(ktcGame & game){
	cout << "Exiting Pause state.\n";
}

bool Pause::ExecuteMessage(ktcGame & game, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_RESUME_GAME:	cout << "Starting the game.\n";
						return true;
		case KTC_END_GAME:	cout << "Ending the game\n";
							return true;
	}
	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for RoundBreak
RoundBreak* RoundBreak::getInstance(){

  static RoundBreak only_inst;

  return &only_inst;
}

void RoundBreak::Enter(ktcGame & game){
	cout << "Entering RoundBreak state.\n";
	//Game now has their own timer
	game.getBreakTime()->setTime(10000);
	game.getBreakTime()->setLastTime(game.getDevice()->getTimer()->getTime());
	game.getGameHUD()->setGameState(3);

	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void RoundBreak::Execute(ktcGame & game, const irr::ITimer* timer){
	//cout << "Executing RoundBreak state.\n";
	
	//update break timer
	game.getBreakTime()->update(timer);

	//update HUD	
	game.getGameHUD()->getInstance()->updateRoundTimer(
	game.getBreakTime()->getMins(),
	game.getBreakTime()->getSecsSecond(),
	game.getBreakTime()->getSecsFirst());
	//Round Break Time is up	
	if((timer->getTime() - game.getLastTime()) > 10000)	
	{
		//Set last time for offset		
		game.setLastTime(timer->getTime());
		//Change to Pre Play State		
		game.GetFSM()->ChangeState(PrePlay::getInstance());	
	}
	game.update(timer);
}

void RoundBreak::Exit(ktcGame & game){
	cout << "Exiting RoundBreak state.\n";
	game.getGameHUD()->setGunMarker(game.getGameHUD()->getGunMarker()+1);
}

bool RoundBreak::ExecuteMessage(ktcGame & game, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_BEGIN_ROUND:	cout << "Starting the game.\n";
						return true;
		case KTC_END_GAME:	cout << "Ending the game\n";
							return true;
	}
	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for EndersGame
EndersGame* EndersGame::getInstance(){

  static EndersGame only_inst;

  return &only_inst;
}

void EndersGame::Enter(ktcGame & game){
	cout << "Entering EndersGame state.\n";
	game.getGameHUD()->setGameState(4);}
void EndersGame::Execute(ktcGame & game, const irr::ITimer* timer){
	//cout << "Executing EndersGame state.\n";

	//put ChangeState shit here in if conditions
}

void EndersGame::Exit(ktcGame & game){
	cout << "Exiting EndersGame state.\n";
}

bool EndersGame::ExecuteMessage(ktcGame & game, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_END_GAME:	cout << "Ending the game\n";
							return true;
	}
	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}


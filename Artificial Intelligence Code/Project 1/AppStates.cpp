#include "AppStates.h"
#include "MessageHandler.h"

#include <iostream>
using std::cout;


//methods for Init
Init* Init::getInstance(){

  static Init only_inst;

  return &only_inst;
}

void Init::Enter(App & app){
	cout << "Entering Init state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void Init::Execute(App & app, const irr::ITimer* timer){
	cout << "Executing Init state.\n";

	//put ChangeState shit here in if conditions
}

void Init::Exit(App & app){
	cout << "Exiting Init state.\n";
}

bool Init::ExecuteMessage(App & app, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_KILL:	cout << "I killed you sucka.\n";
						return true;
		case KTC_SPOTTED:	cout << "I've been spotted, gotta run!\n";
							return true;
	}

	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for MainMenu
MainMenu* MainMenu::getInstance(){

  static MainMenu only_inst;

  return &only_inst;
}

void MainMenu::Enter(App & app){
	cout << "Entering MainMenu state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void MainMenu::Execute(App & app, const irr::ITimer* timer){
	cout << "Executing MainMenu state.\n";

	//put ChangeState shit here in if conditions
}

void MainMenu::Exit(App & app){
	cout << "Exiting MainMenu state.\n";
}

bool MainMenu::ExecuteMessage(App & app, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_KILL:	cout << "I killed you sucka.\n";
						return true;
		case KTC_SPOTTED:	cout << "I've been spotted, gotta run!\n";
							return true;
	}

	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for PlayGame
PlayGame* PlayGame::getInstance(){

  static PlayGame only_inst;

  return &only_inst;
}

void PlayGame::Enter(App & app){
	cout << "Entering PlayGame state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void PlayGame::Execute(App & app, const irr::ITimer* timer){
	cout << "Executing PlayGame state.\n";

	//put ChangeState shit here in if conditions
}

void PlayGame::Exit(App & app){
	cout << "Exiting PlayGame state.\n";
}

bool PlayGame::ExecuteMessage(App & app, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_KILL:	cout << "I killed you sucka.\n";
						return true;
		case KTC_SPOTTED:	cout << "I've been spotted, gotta run!\n";
							return true;
	}

	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for ConfigGame
ConfigGame* ConfigGame::getInstance(){

  static ConfigGame only_inst;

  return &only_inst;
}

void ConfigGame::Enter(App & app){
	cout << "Entering ConfigGame state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void ConfigGame::Execute(App & app, const irr::ITimer* timer){
	cout << "Executing ConfigGame state.\n";

	//put ChangeState shit here in if conditions
}

void ConfigGame::Exit(App & app){
	cout << "Exiting ConfigGame state.\n";
}

bool ConfigGame::ExecuteMessage(App & app, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_KILL:	cout << "I killed you sucka.\n";
						return true;
		case KTC_SPOTTED:	cout << "I've been spotted, gotta run!\n";
							return true;
	}

	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

//methods for ExitApp
ExitApp* ExitApp::getInstance(){

  static ExitApp only_inst;

  return &only_inst;
}

void ExitApp::Enter(App & app){
	cout << "Entering ExitApp state.\n";
	//use MsgHandler->postMessage() here to post a message to all other players (use for loop or some shit)
}

void ExitApp::Execute(App & app, const irr::ITimer* timer){
	cout << "Executing ExitApp state.\n";

	//put ChangeState shit here in if conditions
}

void ExitApp::Exit(App & app){
	cout << "Exiting ExitApp state.\n";
}

bool ExitApp::ExecuteMessage(App & app, const Message *msg){
	switch(msg->messageType){
		//put whatever message types you need here, these are just some examples until we change shit up
		case KTC_KILL:	cout << "I killed you sucka.\n";
						return true;
		case KTC_SPOTTED:	cout << "I've been spotted, gotta run!\n";
							return true;
	}

	//false gets returned if no cases we're matched (we couldn't handle the message)
	return false;
}

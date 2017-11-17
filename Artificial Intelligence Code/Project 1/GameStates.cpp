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
	game.getPreTime()->setTime(5000);
	game.getPreTime()->setLastTime(game.getDevice()->getTimer()->getTime());
	game.getGameHUD()->setGameState(0);
}

void PrePlay::Execute(ktcGame & game, const irr::ITimer* timer){
	cout << "Executing PrePlay state.\n";
	//update round timer
	game.getPreTime()->update(timer);

	//update HUD	game.getGameHUD()->getInstance()->updateRoundTimer(		game.getPreTime()->getMins(),		game.getPreTime()->getSecsSecond(),		game.getPreTime()->getSecsFirst());		//Pre-Play Time is up	if((timer->getTime() - game.getLastTime()) > 5000)	{		//Set last time for offset		game.setLastTime(timer->getTime());		//Change to Play State		game.GetFSM()->ChangeState(Play::getInstance());	}	game.getDevice()->getVideoDriver()->beginScene(true, true, video::SColor(255,100,101,140));	game.getSceneManager()->drawAll();  //draw 3d objects	game.getGameHUD()->render();	game.getPlayer()->getGun().render();	game.getDevice()->getVideoDriver()->endScene();//end drawing	//put ChangeState shit here in if conditions}

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

void Play::Execute(ktcGame & game, const irr::ITimer* timer){
	cout << "Executing Play state.\n";

		//update round timer	game.getRoundTime()->update(timer);	//update HUD	game.getGameHUD()->getInstance()->updateRoundTimer(		game.getRoundTime()->getMins(),		game.getRoundTime()->getSecsSecond(),		game.getRoundTime()->getSecsFirst());	//if time is up, then round robin shit so that we get new predator and prey	if(game.getRoundTime()->getTime() <= 0)	{		game.RoundRobin(*(game.getPlayerList()));		for(int i = 0; i < game.getPlayerList()->size(); i++)		{			(*game.getPlayerList())[i]->setInvTimer(5000);			(*game.getPlayerList())[i]->setTimer(60000);		}		//Set last update before state change		game.setLastTime(timer->getTime());		//Change to Round Over State		game.GetFSM()->ChangeState(RoundBreak::getInstance());	}		/*if(agent2.getPlayerType() == PREY) 		std::cout << "I'm an agent and i'm PREY\n";	else std::cout << "I'm an agent and i'm a PREDATOR\n";		if(plyr.getPlayerType() == PREY) 		std::cout << "I'm a player and i'm PREY\n";	else std::cout << "I'm a player and i'm a PREDATOR\n";*/	game.getDevice()->getVideoDriver()->beginScene(true, true, video::SColor(255,100,101,140));	game.getSceneManager()->drawAll();  //draw 3d objects	game.getGameHUD()->render();	game.getPlayer()->getGun().render();	game.getDevice()->getVideoDriver()->endScene();//end drawing 	if(InputHandler::getInstance()->unprocessedMouseMessageLMB)	{		/*#ifdef NODE_MESH_GENERATOR		graph.addNode(camera->getPosition());		#endif				#ifdef SPAWN_POINT_CREATOR		this->spawnPointList.push_back(camera->getPosition());		FILE *fp = fopen("SPAWN_POINTS.txt", "a");		fprintf(fp, "%f %f %f\n", this->camera->getPosition().X, this->camera->getPosition().Y, this->camera->getPosition().Z);		fclose(fp);		#endif		#ifdef COVER_OBJECT_GENERATOR		irr::scene::ISceneNode* t= smgr->addCubeSceneNode(1);		t->setPosition(camera->getPosition());		t->setScale(vector3df(50,75,50));		coverObjectList.push_back(camera->getPosition());		t->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/crate.jpg"));		t->setMaterialTexture(1, device->getVideoDriver()->getTexture("../media/cratebump.jpg"));		t->setMaterialFlag(video::EMF_LIGHTING, true);		t->setMaterialFlag(video::EMF_FOG_ENABLE, true);		t->setMaterialType(video::EMT_LIGHTMAP_LIGHTING_M4);		#endif */		//Gun Mechanics - Make sure animation is complete		if(game.getPlayer()->getGun().isReady())		{			MessageHandler::getInstance()->postMessage(KTC_PLAYER_LEFT_MOUSE_CLICK, 0, &game, &game.getPlayer()->getGun(), timer);				//Make sure gun has passed the firing time limitation			if(game.getGameHUD()->getGunReady())			{				for(int i = 0; i < game.getEntities().size(); i++)				{					if(game.pointing() == (game.getEntities())[i]->getSceneNode())					{						MessageHandler::getInstance()->postMessage(KTC_KILL, 0, &game, (game.getEntities())[i], game.getDevice()->getTimer());						break;					}				}			}		}		if(game.pointing() == game.getCan()->getSceneNode() && (game.getPlayer()->getSceneNode()->getPosition() - game.getCan()->getSceneNode()->getPosition()).getLength() <= 100.0f)		{			for(int i = 0; i < game.getEntities().size(); i++)			{				MessageHandler::getInstance()->postMessage(KTC_REVIVE, 0, &game, (game.getEntities())[i], game.getDevice()->getTimer());			}		}		InputHandler::getInstance()->unprocessedMouseMessageLMB = false;	}	if(InputHandler::getInstance()->unprocessedMouseMessageRMB)	{		//graph.output();		#ifdef COVER_OBJECT_GENERATOR		FILE *fp = fopen("COVER_OBJECTS.txt", "w");		for(int i = 0; i < this->coverObjectList.size(); i++)		{			fprintf(fp, "%f %f %f\n", coverObjectList[i].X, coverObjectList[i].Y, coverObjectList[i].Z);		}		fclose(fp);		#endif		InputHandler::getInstance()->unprocessedMouseMessageRMB = false;	}	//Toggle the render output of the Debug visible objects	if(InputHandler::getInstance()->isTKeyPressed())	{		game.getMapGraph()->toggleDebugOutput(!game.getMapGraph()->isDebugOutput());	}	//Toggle the render output of the GUI scoring mechanism	//if(InputHandler::getInstance()->isTabKeyPressed()){	//	graph.toggleScoreOutput(!graph.isScoreOutput());	//}	game.getCan()->update(timer);	game.getPlayer()->update(timer);	#ifndef NODE_MESH_GENERATOR	static mapGraph* mintree = game.getMapGraph()->minimumSpanningTree(0);	switch(game.getDMode())	{		case NONE:			break;		case FULLGRAPH: 			game.getMapGraph()->render(game.getDevice()->getVideoDriver());			break;		case MINSPANNINGTREE: 			mintree->render(game.getDevice()->getVideoDriver());			break;	}	for(int i = 0; i < (int)game.getEntities().size();i++)	{		if((game.getEntities())[i])		{			(game.getEntities())[i]->update(timer);			if(game.getMapGraph()->isDebugOutput())			{				//	entities[i]->drawPieSlices(device->getVideoDriver());			}		}	}	#endif}
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
	cout << "Executing Pause state.\n";

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
	cout << "Executing RoundBreak state.\n";
	
	//update break timer
	game.getBreakTime()->update(timer);

	//update HUD	game.getGameHUD()->getInstance()->updateRoundTimer(		game.getBreakTime()->getMins(),		game.getBreakTime()->getSecsSecond(),		game.getBreakTime()->getSecsFirst());	//Round Break Time is up	if((timer->getTime() - game.getLastTime()) > 10000)	{		//Set last time for offset		game.setLastTime(timer->getTime());		//Change to Pre Play State		game.GetFSM()->ChangeState(PrePlay::getInstance());	}	game.getDevice()->getVideoDriver()->beginScene(true, true, video::SColor(255,100,101,140));	game.getSceneManager()->drawAll();  //draw 3d objects	game.getGameHUD()->render();	game.getPlayer()->getGun().render();	game.getDevice()->getVideoDriver()->endScene();//end drawing	}
void RoundBreak::Exit(ktcGame & game){
	cout << "Exiting RoundBreak state.\n";
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
	cout << "Executing EndersGame state.\n";

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


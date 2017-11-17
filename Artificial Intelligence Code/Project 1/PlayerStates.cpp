#include "player.h"
#include "PlayerStates.h"
#include "MessageHandler.h"

#include <iostream>
using std::cout;

PlayerPlay* PlayerPlay::GetInstance(){
	static PlayerPlay only_inst;
	return &only_inst;

}
 
void PlayerPlay::Enter(player & plyr){
	//set anim to stand
}

void PlayerPlay::Execute(player & plyr, const irr::ITimer* timer){

}

void PlayerPlay::Exit(player & plyr){

}

bool PlayerPlay::ExecuteMessage(player & plyr, const Message * msg){

	//false always gets returned here because we don't want to handle any messages in the Play state
	return false;
}

PlayerWait* PlayerWait::GetInstance(){
	static PlayerWait only_inst;
	return &only_inst;

}
 
void PlayerWait::Enter(player & plyr){
	std::cout << "Entering player wait state.\n";

	
	//set the fog properties
	plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,0,0,0), true, 0,0, 0, true, true);
	std::cout <<"I got fog.\n";

	start = plyr.getDevice()->getTimer()->getTime();
	finish = start + 10000;
	
	
}

void PlayerWait::Execute(player & plyr, const irr::ITimer* timer){
	
	if(plyr.getDevice()->getTimer()->getTime() >= 10000)
		plyr.GetFSM()->ChangeState(PlayerPlay::GetInstance());

	plyr.setPosition(plyr.getSpawnPoint());

	
	if(plyr.getDevice()->getTimer()->getTime() < finish){
	
		int diff = plyr.getDevice()->getTimer()->getTime() - start;
		int range = -1500 + 2250.0f    *((double)diff / (double)((finish- start)));

		int color =  25.0f    *((double)diff / (double)((finish- start))); 

		if(range >= 0){
			plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255, color,color,color), true, 0,range,0,true, true);
		}
		else{
			plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor( 255, color, color, color), true, 0, 1, 0, true, true);
		}
	}
	
}

void PlayerWait::Exit(player & plyr){

}

bool PlayerWait::ExecuteMessage(player & plyr, const Message * msg){

	//false always gets returned here because we don't want to handle any messages in the wait state
	return false;
}
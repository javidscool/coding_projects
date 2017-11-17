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
	plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,25,25,25), true, 0,750, 0, true, true);//set the fog properties
}

void PlayerPlay::Execute(player & plyr, const irr::ITimer* timer){

}

void PlayerPlay::Exit(player & plyr){

}

bool PlayerPlay::ExecuteMessage(player & plyr, const Message * msg){

	switch(msg->messageType){
	
	
		case KTC_KILL: if(plyr.pl_inv.getTime() > 0){
					   return true;
					   }
					   plyr.GetFSM()->ChangeState(PlayerDie::GetInstance());
					   return true;
	


	
	}

	//false always gets returned here because we don't want to handle any messages in the Play state
	return false;
}



PlayerDie* PlayerDie::GetInstance(){

	static PlayerDie only_inst;
	return &only_inst;

}


void PlayerDie::Enter(player & plyr){


	start = plyr.getDevice()->getTimer()->getTime();
	finish = start + 10000;
	temp = plyr.getSceneNode()->getPosition();

}


void PlayerDie::Execute(player &plyr, const irr::ITimer* timer){


	plyr.getSceneNode()->setPosition(temp);

	static int bloodier = 25;
	static int decrease = 25;


	if(plyr.getDevice()->getTimer()->getTime()< finish){
	
	
		if(decrease == 0){
		
			if(bloodier == 255)
				plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,255,0,0),true, 0,750,0,true,true);
			else{
			
				plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,bloodier,0,0),true,0,750, 0, true, true);
				bloodier+=5;
			
			}
		
		
		}else{
			plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,bloodier, decrease, decrease), true, 0, 750, 0, true, true);
			bloodier+=5;
			decrease-=1;
		}
	
	}



}

void PlayerDie::Exit(player& plyr){

	plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,25,25,25), true, 0, 750, 0, true, true);

}

bool PlayerDie::ExecuteMessage(player & plyr, const Message* msg){

	switch(msg->messageType){
	
	
		case KTC_REVIVE:
			plyr.GetFSM()->ChangeState(PlayerPlay::GetInstance());
			return true;
	
	}
return false;

}









PlayerWait* PlayerWait::GetInstance(){
	static PlayerWait only_inst;
	return &only_inst;

}
 
void PlayerWait::Enter(player & plyr){
	std::cout << "Entering player wait state.\n";
		
}

void PlayerWait::Execute(player & plyr, const irr::ITimer* timer){	
	
}

void PlayerWait::Exit(player & plyr){

}

bool PlayerWait::ExecuteMessage(player & plyr, const Message * msg){

	//false always gets returned here because we don't want to handle any messages in the wait state
	return false;
}


PlayerStart* PlayerStart::GetInstance(){
	static PlayerStart only_inst;
	return &only_inst;

}
 
void PlayerStart::Enter(player & plyr){

//return;
	std::cout << "Entering player wait state.\n";

	//set the fog properties
	plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,0,0,0), true, 0,0, 0, true, true);
	std::cout <<"I got fog.\n";

	start = plyr.getDevice()->getTimer()->getTime();
	finish = start + 10000;
	
	temp = plyr.getSceneNode()->getPosition();
}

void PlayerStart::Execute(player & plyr, const irr::ITimer* timer){
	

	//	plyr.GetFSM()->ChangeState(PlayerPlay::GetInstance());


	if(plyr.getDevice()->getTimer()->getTime() >= finish)
		plyr.GetFSM()->ChangeState(PlayerPlay::GetInstance());

	plyr.getSceneNode()->setPosition(temp);

	
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

void PlayerStart::Exit(player & plyr){
	plyr.getDevice()->getVideoDriver()->setFog(irr::video::SColor(255,25,25,25), true, 0,750, 0, true, true);//set the fog properties

}

bool PlayerStart::ExecuteMessage(player & plyr, const Message * msg){

	//false always gets returned here because we don't want to handle any messages in the wait state
	return false;
}
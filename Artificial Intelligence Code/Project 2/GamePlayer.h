#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_

#include "Timer.h"
#include "physicsObject.h"

enum GamePlayer_Type{PREDATOR, PREY};

class GamePlayer : public physicsObject{


	static std::vector<GamePlayer*>* playerList;

protected:
	
	GamePlayer* IT;

	//this delegates a game player to one of two types: PREDATOR or PREY
	GamePlayer_Type type;

	irr::core::vector3df spawnPoint;

	double MAXSPEED;

public:


void setIt(GamePlayer* p){IT = p;}
bool GamePlayer::isSafe();
GamePlayer* getIt(){return IT;}
	
	
	inline static void setPlayerList(std::vector<GamePlayer*>* abc){playerList = abc;}
	inline static std::vector<GamePlayer*>* getPlayerList(){return playerList;}

	//every Agent now has their own timer
	Timer pl_time;

	//invincibility timer
	Timer pl_inv;

	GamePlayer(irr::core::vector3df sp = irr::core::vector3df(0,0,0) , Timer pl_t = 0, Timer pl_in = 0, GamePlayer_Type ty = PREY);

	//spawnPoint getter/setter
	inline irr::core::vector3df getSpawnPoint(){ return spawnPoint; }
	inline void setSpawnPoint(irr::core::vector3df sp){ spawnPoint = sp; }

	//getter and setter for player speed
	inline double getSpeed(){ return MAXSPEED; }
	virtual void setSpeed() = 0;

	//pl_time player type getter/setter
	inline Timer getTimer(){ return pl_time; }
	inline void setTimer(int time){ pl_time.setTime(time); }

	//pl_inv player type getter/setter
	inline Timer getInvTimer(){ return pl_inv; }
	inline void setInvTimer(int time){ pl_inv.setTime(time); }

	//game player type getter/setter
	inline GamePlayer_Type getPlayerType(){ return type; }
	virtual void setPlayerType(GamePlayer_Type T) = 0;


	virtual irr::core::vector3df getLineOfSight() = 0;
};

#endif


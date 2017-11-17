#ifndef GAME_HUD
#define GAME_HUD

#include "irrlicht.h"

using namespace irr;
using namespace video;

//Object that represents all Scene Nodes Assembled for in-game HUD and UI
class gameHUD {

private:
	video::IVideoDriver* driver;

	int x1Bar, y1Bar, x2Bar, y2Bar;

	enum {PRE_PLAY,PLAY,PAUSE,BREAK,END_GAME};	
	int gameState;
	int whoHasGun;

	int	maxShotTimerBarValue; //size of the timer bar
	int	roundTimer;			  //Current Round Time, updated by clock function
	double deltaShotTimerBar;  //difference scalar
	double shotTimerBarValue;  //current bar value
	double screenX, screenY;
	bool gunTimerReady;		//current gun readiness
	bool showScore;			//score display flag
   
	//Texture Storage for random GUI HUD elements
   //Assignments:
   //	0 : Gun Timer Bar
   //	1 : Crosshair
   //   2 : Clock Pre-Game Label
   //   3 : Clock Play Label
   //   4 : Clock Pause Label
   //   5 : Clock Round Over Label
   video::ITexture* GUITextures[10];

   //Texture Storage for character GUI HUD elements
   //Assignments:
   //	0-9 : Numbers zero through nine
   //	10  : Colon Character 
   video::ITexture* CharTextures[11];

   //Texture Storage for fixed position clock characters
   //Assignments:
   //	0 : Minutes (zero to nine)
   //	1 : Seconds (1x10^1)
   //	2 : Seconds (1x10^0)
   //   3 : Clock Label
   video::ITexture* ClockTextures[4];

   //Texture Storage for score counters
   //Assignments:
   //	0   : Label
   //	1-5	: Player Identifiers
   //	6	: gun identifier
   //   7   : blank (transparent)
   //  10-19: Score Numbers 0-9
   video::ITexture* ScoreTextures[20];

   //Texture Storage for each player's score
   //Assignments:
   //   0-5 : Player's score in order of increasing magnitude
   video::ITexture* Player1ScoreTextures[6];
   video::ITexture* Player2ScoreTextures[6];
   video::ITexture* Player3ScoreTextures[6];
   video::ITexture* Player4ScoreTextures[6];
   video::ITexture* Player5ScoreTextures[6];

   gameHUD(); //constructor
   ~gameHUD(); //destructor :O

public:

   static gameHUD* getInstance(); //returns an instance of the gameHUD
   void render(); //render the HUD, must be called AFTER the 3D draw, or it  wont show
   void setShotTimerBarValue(int shotTimerValue); //change bar values
   void updateRoundTimer(irr::u32 numMins, irr::u32 numSecsOrder2, irr::u32 numSecsOrder1); //changes game timer value

   void setVideoDriver (video::IVideoDriver* videoDriver);
   inline void setGameState(int stateToSet) {gameState = stateToSet;}
   inline void setGunMarker(int playerNum) {whoHasGun = playerNum;}
   inline int getGunMarker() {return whoHasGun;}
   void loadTextures(); //loads all relevant textures into private member variables
   void setGunReady(bool ready); //sets gun Readiness
   bool getGunReady(); //returns true if gun is ready to be fired, false otherwise
   void updateScores(int playerScores[]);
   void drawScores(); //renders the scoreboard
   void drawMarker(int playerNum);
}; 

#endif
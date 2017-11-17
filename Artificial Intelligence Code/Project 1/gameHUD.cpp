#include "gameHUD.h"


using namespace irr;
using namespace video;

gameHUD::gameHUD()
{
	//set gun to ready
	gunTimerReady = true;

	//set visibility of score graphics
	showScore = false;
       
   //shot timer bar size
   x1Bar = 52;
   y1Bar = 37;
   x2Bar = 55;
   y2Bar = 53;
       
   //max bar values
   maxShotTimerBarValue = 100;

   //current bar values
   shotTimerBarValue = 100;

   //scalar delta for bar value
   deltaShotTimerBar = shotTimerBarValue;

} 

gameHUD::~gameHUD()
{
}

void gameHUD::render(){
	switch(gameState)
	{
		case PRE_PLAY:
			break;
		case PLAY:
			float delta; //status bar variation

			//changes the bar value from X to Y smoothly
			if ((int)deltaShotTimerBar != (int)shotTimerBarValue){
				delta = shotTimerBarValue - deltaShotTimerBar;
				if (delta > 0)
					shotTimerBarValue -= 0.2;
				if (delta < 0)
				shotTimerBarValue += 0.2;
			}

			//draw the score Table	
			driver->draw2DImage(ScoreTextures[0], core::position2d<s32>((screenX)-192, (screenY/24)), core::rect<s32>(0,0,192,92), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[1], core::position2d<s32>((screenX)-168, (screenY/24)+60), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[2], core::position2d<s32>((screenX)-168, (screenY/24)+84), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[3], core::position2d<s32>((screenX)-168, (screenY/24)+108), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[4], core::position2d<s32>((screenX)-168, (screenY/24)+132), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[5], core::position2d<s32>((screenX)-168, (screenY/24)+156), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+60), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);

			//draw the clock	
			driver->draw2DImage(ClockTextures[0], core::position2d<s32>((screenX/24)-36, (screenY/24)), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(CharTextures[10], core::position2d<s32>((screenX/24)-12, (screenY/24)), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[1], core::position2d<s32>((screenX/24)+12, (screenY/24)), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[2], core::position2d<s32>((screenX/24)+48, (screenY/24)), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);

			//draw the cross hair
			driver->draw2DImage(GUITextures[1], core::position2d<s32>((screenX/2)-16, (screenY/2)-16), core::rect<s32>(0,0,32,32), 0, video::SColor(255,255,255,255), true);
       
			//draws the timer bar according to its value (green for high, decreasing to yellow till red), and some others rectangles for a cool look
			driver->draw2DImage(GUITextures[0], core::position2d<s32>(screenX - 341, screenY - 73), core::rect<s32>(0,0,341,73), 0, video::SColor(255,255,255,255), true);

			driver->draw2DRectangle(video::SColor(255, 100, 100, 100), core::rect<s32>((screenX - 305), (screenY - 45), ((maxShotTimerBarValue*2.7)+ (screenX - 305)), (screenY - 25)));

			driver->draw2DRectangle(video::SColor(255, 125, 125, 125), core::rect<s32>((screenX - 305)+1, (screenY - 45)+1, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-1), (screenY - 25)-1));

			driver->draw2DRectangle(video::SColor(255, 150, 150, 150), core::rect<s32>((screenX - 305)+3, (screenY - 45)+3, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-3), (screenY - 25)-3));

			driver->draw2DRectangle(core::rect<s32>((screenX - 305)+3, (screenY - 45)+3, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-3), (screenY - 25)-3),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55-150, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55-150, 0)); 

			break;
		case PAUSE:
			break;
		case BREAK:
			break;
		case END_GAME:
			break;
		default:
			break;
	}
} 

void gameHUD::setShotTimerBarValue(int shotTimerValue){
   deltaShotTimerBar = shotTimerValue;
}void gameHUD::updateRoundTimer(irr::u32 numMins, irr::u32 numSecsOrder2, irr::u32 numSecsOrder1){	//Changes clock texture to appropriate minutes-number texture	if(numMins > 9)		ClockTextures[0] = CharTextures[9];	else		ClockTextures[0] = CharTextures[numMins];	//Changes clock texture to appropriate seconds-number texture (second order)	if(numSecsOrder2 > 9)		ClockTextures[1] = CharTextures[9];	else		ClockTextures[1] = CharTextures[numSecsOrder2];	//Changes clock texture to appropriate seconds-number texture (first order)	if(numSecsOrder1 > 9)		ClockTextures[2] = CharTextures[9];	else		ClockTextures[2] = CharTextures[numSecsOrder1];}void gameHUD::setVideoDriver (video::IVideoDriver* videoDriver){	//set the internal driver variable to the input	driver = videoDriver;	//Get screen dimensions	core::dimension2d<s32> screenSize = driver->getScreenSize(); 	screenX = screenSize.Width;	screenY = screenSize.Height; 	//load relevant textures into private member functions	loadTextures();}/*void gameHUD::setGameContext(ktcGame *game){	//set internal game context to the game pointer	gameContext = game;}*///returns the instance of the gameHUD classgameHUD* gameHUD::getInstance(){	static gameHUD instance;		return &instance;}void gameHUD::loadTextures(){	//Texture Loading   //load some texture for spellbar and make it transparent with ColorKey 0 (black)   GUITextures[0] = driver->getTexture("../Media/Textures/spellbar_smaller.bmp");   driver->makeColorKeyTexture(GUITextures[0], video::SColor(0,0,0,0));   //load texture for crosshair   GUITextures[1] = driver->getTexture("../Media/crosshair.bmp");   driver->makeColorKeyTexture(GUITextures[1], video::SColor(0,0,0,0));   //Load texture for Round Start Message   GUITextures[2] = driver->getTexture("../Media/Textures/GO_Message.bmp");   driver->makeColorKeyTexture(GUITextures[2], video::SColor(0,0,0,0));   //Load texture for Round End Message   GUITextures[3] = driver->getTexture("../Media/Textures/ROUND_OVER_Message.bmp");   driver->makeColorKeyTexture(GUITextures[3], video::SColor(0,0,0,0));   //load textures for characters   //Number zero   CharTextures[0] = driver->getTexture("../Media/Textures/zero.bmp");   driver->makeColorKeyTexture(CharTextures[0], video::SColor(0,0,0,0));   //Number one   CharTextures[1] = driver->getTexture("../Media/Textures/one.bmp");   driver->makeColorKeyTexture(CharTextures[1], video::SColor(0,0,0,0));   //Number two   CharTextures[2] = driver->getTexture("../Media/Textures/two.bmp");   driver->makeColorKeyTexture(CharTextures[2], video::SColor(0,0,0,0));   //Number three   CharTextures[3] = driver->getTexture("../Media/Textures/three.bmp");   driver->makeColorKeyTexture(CharTextures[3], video::SColor(0,0,0,0));   //Number four   CharTextures[4] = driver->getTexture("../Media/Textures/four.bmp");   driver->makeColorKeyTexture(CharTextures[4], video::SColor(0,0,0,0));   //Number five   CharTextures[5] = driver->getTexture("../Media/Textures/five.bmp");   driver->makeColorKeyTexture(CharTextures[5], video::SColor(0,0,0,0));   //Number six   CharTextures[6] = driver->getTexture("../Media/Textures/six.bmp");   driver->makeColorKeyTexture(CharTextures[6], video::SColor(0,0,0,0));   //Number seven   CharTextures[7] = driver->getTexture("../Media/Textures/seven.bmp");   driver->makeColorKeyTexture(CharTextures[7], video::SColor(0,0,0,0));   //Number eight   CharTextures[8] = driver->getTexture("../Media/Textures/eight.bmp");   driver->makeColorKeyTexture(CharTextures[8], video::SColor(0,0,0,0));   //Number nine   CharTextures[9] = driver->getTexture("../Media/Textures/nine.bmp");   driver->makeColorKeyTexture(CharTextures[9], video::SColor(0,0,0,0));   //Colon Character   CharTextures[10] = driver->getTexture("../Media/Textures/colon.bmp");   driver->makeColorKeyTexture(CharTextures[10], video::SColor(0,0,0,0));   //Load textures for score keeping   //label   ScoreTextures[0] = driver->getTexture("../Media/Textures/scores.bmp");   driver->makeColorKeyTexture(ScoreTextures[0], video::SColor(0,0,0,0));   //Player 1   ScoreTextures[1] = driver->getTexture("../Media/Textures/player1.bmp");   driver->makeColorKeyTexture(ScoreTextures[1], video::SColor(0,0,0,0));   //Player 2   ScoreTextures[2] = driver->getTexture("../Media/Textures/player2.bmp");   driver->makeColorKeyTexture(ScoreTextures[2], video::SColor(0,0,0,0));   //Player 3   ScoreTextures[3] = driver->getTexture("../Media/Textures/player3.bmp");   driver->makeColorKeyTexture(ScoreTextures[3], video::SColor(0,0,0,0));   //Player 4   ScoreTextures[4] = driver->getTexture("../Media/Textures/player4.bmp");   driver->makeColorKeyTexture(ScoreTextures[4], video::SColor(0,0,0,0));   //Player 5   ScoreTextures[5] = driver->getTexture("../Media/Textures/player5.bmp");   driver->makeColorKeyTexture(ScoreTextures[5], video::SColor(0,0,0,0));   //Gun Identifier   ScoreTextures[6] = driver->getTexture("../Media/Textures/gun.bmp");   driver->makeColorKeyTexture(ScoreTextures[6], video::SColor(0,0,0,0));   //Initialize clock   updateRoundTimer(0,0,0);}void gameHUD::setGunReady(bool ready){	gunTimerReady = ready;}bool gameHUD::getGunReady(){	return gunTimerReady;}
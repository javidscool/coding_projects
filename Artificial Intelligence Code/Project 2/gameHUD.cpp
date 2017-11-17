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

   //who has the fucking gun?
   whoHasGun = 1;

} 

gameHUD::~gameHUD()
{
}

void gameHUD::render(){
	
	switch(gameState)
	{
		case PRE_PLAY:
			//draw the clock bar
			ClockTextures[3] = GUITextures[2];
			driver->draw2DImage(ClockTextures[3], core::position2d<s32>((screenX/24)-36, (screenY/24)), core::rect<s32>(0,0,192,48), 0, video::SColor(255,255,255,255), true);
			//draw the clock	
			driver->draw2DImage(ClockTextures[0], core::position2d<s32>((screenX/24)-24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(CharTextures[10], core::position2d<s32>((screenX/24), (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[1], core::position2d<s32>((screenX/24)+24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[2], core::position2d<s32>((screenX/24)+60, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			break;
		case PLAY:
			/*float delta; //status bar variation

			//changes the bar value from X to Y smoothly
			if ((int)deltaShotTimerBar != (int)shotTimerBarValue){
				delta = shotTimerBarValue - deltaShotTimerBar;
				if (delta > 0)
					shotTimerBarValue -= 0.2;
				if (delta < 0)
				shotTimerBarValue += 0.2;
			}*/

			//draw scores
			drawScores();

			//draw the clock bar
			ClockTextures[3] = GUITextures[3];
			driver->draw2DImage(ClockTextures[3], core::position2d<s32>((screenX/24)-36, (screenY/24)), core::rect<s32>(0,0,192,48), 0, video::SColor(255,255,255,255), true);

			//draw the clock	
			driver->draw2DImage(ClockTextures[0], core::position2d<s32>((screenX/24)-24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(CharTextures[10], core::position2d<s32>((screenX/24), (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[1], core::position2d<s32>((screenX/24)+24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[2], core::position2d<s32>((screenX/24)+60, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			
			//draw the cross hair
			driver->draw2DImage(GUITextures[1], core::position2d<s32>((screenX/2)-16, (screenY/2)-16), core::rect<s32>(0,0,32,32), 0, video::SColor(255,255,255,255), true);
       
			/*//draws the timer bar according to its value (green for high, decreasing to yellow till red), and some others rectangles for a cool look
			driver->draw2DImage(GUITextures[0], core::position2d<s32>(screenX - 341, screenY - 73), core::rect<s32>(0,0,341,73), 0, video::SColor(255,255,255,255), true);

			driver->draw2DRectangle(video::SColor(255, 100, 100, 100), core::rect<s32>((screenX - 305), (screenY - 45), ((maxShotTimerBarValue*2.7)+ (screenX - 305)), (screenY - 25)));

			driver->draw2DRectangle(video::SColor(255, 125, 125, 125), core::rect<s32>((screenX - 305)+1, (screenY - 45)+1, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-1), (screenY - 25)-1));

			driver->draw2DRectangle(video::SColor(255, 150, 150, 150), core::rect<s32>((screenX - 305)+3, (screenY - 45)+3, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-3), (screenY - 25)-3));

			driver->draw2DRectangle(core::rect<s32>((screenX - 305)+3, (screenY - 45)+3, ((maxShotTimerBarValue*2.7)+ (screenX - 305)-3), (screenY - 25)-3),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55-150, 0),

                     video::SColor(255, 255-shotTimerBarValue*2.55, shotTimerBarValue*2.55-150, 0)); */

			break;
		case PAUSE:
			//draw the clock bar
			ClockTextures[3] = GUITextures[5];
			driver->draw2DImage(ClockTextures[3], core::position2d<s32>((screenX/24)-36, (screenY/24)), core::rect<s32>(0,0,192,48), 0, video::SColor(255,255,255,255), true);
			break;
		case BREAK:
			//draw the clock bar
			ClockTextures[3] = GUITextures[3];
			driver->draw2DImage(ClockTextures[3], core::position2d<s32>((screenX/24)-36, (screenY/24)), core::rect<s32>(0,0,192,48), 0, video::SColor(255,255,255,255), true);
			//draw the clock	
			driver->draw2DImage(ClockTextures[0], core::position2d<s32>((screenX/24)-24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(CharTextures[10], core::position2d<s32>((screenX/24), (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[1], core::position2d<s32>((screenX/24)+24, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			driver->draw2DImage(ClockTextures[2], core::position2d<s32>((screenX/24)+60, (screenY/24)+48), core::rect<s32>(0,0,48,48), 0, video::SColor(255,255,255,255), true);
			
			//draw scores
			drawScores();

			//draw gun marker
			drawMarker(whoHasGun);

			break;
		case END_GAME:
			break;
		default:
			break;
	}
}
void gameHUD::drawMarker(int playerNum)
{
	//draws the gun marker based on who has the gun.
	switch(playerNum)
	{
		case 1:
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+60), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);
			break;
		case 2:
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+84), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);
			break;
		case 3:
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+108), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);
			break;
		case 4:
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+132), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);
			break;
		case 5:
			driver->draw2DImage(ScoreTextures[6], core::position2d<s32>((screenX)-200, (screenY/24)+156), core::rect<s32>(0,0,50,50), 0, video::SColor(255,255,255,255), true);
			break;
		default:
			break;
	}
}
void gameHUD::drawScores()
{
	//draw the score Table header	
	driver->draw2DImage(ScoreTextures[0], core::position2d<s32>((screenX)-175, (screenY/24)), core::rect<s32>(0,0,192,92), 0, video::SColor(255,255,255,255), true);
	
	//draw the player labels (96wx48h)
	
	//draw the player's scores
	//player 1 label
	driver->draw2DImage(ScoreTextures[1], core::position2d<s32>((screenX)-168, (screenY/24)+60), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
	//player 1 score
	driver->draw2DImage(Player1ScoreTextures[0], core::position2d<s32>((screenX)-84, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player1ScoreTextures[1], core::position2d<s32>((screenX)-72, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player1ScoreTextures[2], core::position2d<s32>((screenX)-60, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player1ScoreTextures[3], core::position2d<s32>((screenX)-48, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player1ScoreTextures[4], core::position2d<s32>((screenX)-36, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player1ScoreTextures[5], core::position2d<s32>((screenX)-24, (screenY/24)+75), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);

	//player 2 label
	driver->draw2DImage(ScoreTextures[2], core::position2d<s32>((screenX)-168, (screenY/24)+84), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
	//player 2 score
	driver->draw2DImage(Player2ScoreTextures[0], core::position2d<s32>((screenX)-84, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player2ScoreTextures[1], core::position2d<s32>((screenX)-72, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player2ScoreTextures[2], core::position2d<s32>((screenX)-60, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player2ScoreTextures[3], core::position2d<s32>((screenX)-48, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player2ScoreTextures[4], core::position2d<s32>((screenX)-36, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player2ScoreTextures[5], core::position2d<s32>((screenX)-24, (screenY/24)+99), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);

	//player 3 label
	driver->draw2DImage(ScoreTextures[3], core::position2d<s32>((screenX)-168, (screenY/24)+108), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
	//player 3 score
	driver->draw2DImage(Player3ScoreTextures[0], core::position2d<s32>((screenX)-84, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player3ScoreTextures[1], core::position2d<s32>((screenX)-72, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player3ScoreTextures[2], core::position2d<s32>((screenX)-60, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player3ScoreTextures[3], core::position2d<s32>((screenX)-48, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player3ScoreTextures[4], core::position2d<s32>((screenX)-36, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player3ScoreTextures[5], core::position2d<s32>((screenX)-24, (screenY/24)+123), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);

	//player 4 label
	driver->draw2DImage(ScoreTextures[4], core::position2d<s32>((screenX)-168, (screenY/24)+132), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
	//player 4 score
	driver->draw2DImage(Player4ScoreTextures[0], core::position2d<s32>((screenX)-84, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player4ScoreTextures[1], core::position2d<s32>((screenX)-72, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player4ScoreTextures[2], core::position2d<s32>((screenX)-60, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player4ScoreTextures[3], core::position2d<s32>((screenX)-48, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player4ScoreTextures[4], core::position2d<s32>((screenX)-36, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player4ScoreTextures[5], core::position2d<s32>((screenX)-24, (screenY/24)+147), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);

	//player 5 label
	driver->draw2DImage(ScoreTextures[5], core::position2d<s32>((screenX)-168, (screenY/24)+156), core::rect<s32>(0,0,96,48), 0, video::SColor(255,255,255,255), true);
	//player 5 score
	driver->draw2DImage(Player5ScoreTextures[0], core::position2d<s32>((screenX)-84, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player5ScoreTextures[1], core::position2d<s32>((screenX)-72, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player5ScoreTextures[2], core::position2d<s32>((screenX)-60, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player5ScoreTextures[3], core::position2d<s32>((screenX)-48, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player5ScoreTextures[4], core::position2d<s32>((screenX)-36, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
	driver->draw2DImage(Player5ScoreTextures[5], core::position2d<s32>((screenX)-24, (screenY/24)+171), core::rect<s32>(0,0,12,16), 0, video::SColor(255,255,255,255), true);
}

void gameHUD::updateScores(int playerScores[])
{
	int scoreMagnitudes[6];
	int temp, scoreTemp;

	for(int i=0 ; i<5 ; i++)
	{
		scoreTemp = playerScores[i];

		//check for score limits
		if(scoreTemp > 999999)
			scoreTemp = 999999; //set it to the max
		if(scoreTemp < 0)
			scoreTemp = 0; //set it to the min

		//n^5
		temp = scoreTemp/100000;
		scoreMagnitudes[0] = temp;
		scoreTemp -= temp*100000;
		//n^4
		temp = scoreTemp/10000;
		scoreMagnitudes[1] = temp;
		scoreTemp -= temp*10000;
		//n^3
		temp = scoreTemp/1000;
		scoreMagnitudes[2] = temp;
		scoreTemp -= temp*1000;
		//n^2
		temp = scoreTemp/100;
		scoreMagnitudes[3] = temp;
		scoreTemp -= temp*100;
		//n^1
		temp = scoreTemp/10;
		scoreMagnitudes[4] = temp;
		scoreTemp -= temp*10;
		//n^0
		scoreMagnitudes[5] = scoreTemp;

		switch(i)
		{
			case 0:
				Player1ScoreTextures[0] = ScoreTextures[scoreMagnitudes[0]+10];
				Player1ScoreTextures[1] = ScoreTextures[scoreMagnitudes[1]+10];
				Player1ScoreTextures[2] = ScoreTextures[scoreMagnitudes[2]+10];
				Player1ScoreTextures[3] = ScoreTextures[scoreMagnitudes[3]+10];
				Player1ScoreTextures[4] = ScoreTextures[scoreMagnitudes[4]+10];
				Player1ScoreTextures[5] = ScoreTextures[scoreMagnitudes[5]+10];
				break;
			case 1:
				Player2ScoreTextures[0] = ScoreTextures[scoreMagnitudes[0]+10];
				Player2ScoreTextures[1] = ScoreTextures[scoreMagnitudes[1]+10];
				Player2ScoreTextures[2] = ScoreTextures[scoreMagnitudes[2]+10];
				Player2ScoreTextures[3] = ScoreTextures[scoreMagnitudes[3]+10];
				Player2ScoreTextures[4] = ScoreTextures[scoreMagnitudes[4]+10];
				Player2ScoreTextures[5] = ScoreTextures[scoreMagnitudes[5]+10];
				break;
			case 2:
				Player3ScoreTextures[0] = ScoreTextures[scoreMagnitudes[0]+10];
				Player3ScoreTextures[1] = ScoreTextures[scoreMagnitudes[1]+10];
				Player3ScoreTextures[2] = ScoreTextures[scoreMagnitudes[2]+10];
				Player3ScoreTextures[3] = ScoreTextures[scoreMagnitudes[3]+10];
				Player3ScoreTextures[4] = ScoreTextures[scoreMagnitudes[4]+10];
				Player3ScoreTextures[5] = ScoreTextures[scoreMagnitudes[5]+10];
				break;
			case 3:
				Player4ScoreTextures[0] = ScoreTextures[scoreMagnitudes[0]+10];
				Player4ScoreTextures[1] = ScoreTextures[scoreMagnitudes[1]+10];
				Player4ScoreTextures[2] = ScoreTextures[scoreMagnitudes[2]+10];
				Player4ScoreTextures[3] = ScoreTextures[scoreMagnitudes[3]+10];
				Player4ScoreTextures[4] = ScoreTextures[scoreMagnitudes[4]+10];
				Player4ScoreTextures[5] = ScoreTextures[scoreMagnitudes[5]+10];
				break;
			case 4:
				Player5ScoreTextures[0] = ScoreTextures[scoreMagnitudes[0]+10];
				Player5ScoreTextures[1] = ScoreTextures[scoreMagnitudes[1]+10];
				Player5ScoreTextures[2] = ScoreTextures[scoreMagnitudes[2]+10];
				Player5ScoreTextures[3] = ScoreTextures[scoreMagnitudes[3]+10];
				Player5ScoreTextures[4] = ScoreTextures[scoreMagnitudes[4]+10];
				Player5ScoreTextures[5] = ScoreTextures[scoreMagnitudes[5]+10];
				break;
			default:
				break;
		}//switch
	}//for
}
void gameHUD::setShotTimerBarValue(int shotTimerValue){
   deltaShotTimerBar = shotTimerValue;
}

void gameHUD::updateRoundTimer(irr::u32 numMins, irr::u32 numSecsOrder2, irr::u32 numSecsOrder1)
{
	//Changes clock texture to appropriate minutes-number texture
	if(numMins > 9)
		ClockTextures[0] = CharTextures[9];
	else
		ClockTextures[0] = CharTextures[numMins];

	//Changes clock texture to appropriate seconds-number texture (second order)
	if(numSecsOrder2 > 9)
		ClockTextures[1] = CharTextures[9];
	else
		ClockTextures[1] = CharTextures[numSecsOrder2];

	//Changes clock texture to appropriate seconds-number texture (first order)
	if(numSecsOrder1 > 9)
		ClockTextures[2] = CharTextures[9];
	else
		ClockTextures[2] = CharTextures[numSecsOrder1];
}

void gameHUD::setVideoDriver (video::IVideoDriver* videoDriver)
{
	//set the internal driver variable to the input
	driver = videoDriver;

	//Get screen dimensions
	core::dimension2d<s32> screenSize = driver->getScreenSize(); 
	screenX = screenSize.Width;
	screenY = screenSize.Height; 

	//load relevant textures into private member functions
	loadTextures();
}


//returns the instance of the gameHUD class
gameHUD* gameHUD::getInstance(){
	static gameHUD instance;
	
	return &instance;
}

void gameHUD::loadTextures()
{
	//Texture Loading

   //load some texture for spellbar and make it transparent with ColorKey 0 (black)
   GUITextures[0] = driver->getTexture("../Media/Textures/spellbar_smaller.bmp");
   driver->makeColorKeyTexture(GUITextures[0], video::SColor(0,0,0,0));

   //load texture for crosshair
   GUITextures[1] = driver->getTexture("../Media/crosshair.bmp");
   driver->makeColorKeyTexture(GUITextures[1], video::SColor(0,0,0,0));

   //Load texture for Clock Pre-Game Label
   GUITextures[2] = driver->getTexture("../Media/Textures/round_begins_msg.bmp");
   driver->makeColorKeyTexture(GUITextures[2], video::SColor(0,0,0,0));

   //Load texture for Clock Play Label
   GUITextures[3] = driver->getTexture("../Media/Textures/time_remaining_msg.bmp");
   driver->makeColorKeyTexture(GUITextures[3], video::SColor(0,0,0,0));

   //Load texture for Clock Pause Label
   GUITextures[4] = driver->getTexture("../Media/Textures/pause_msg.bmp");
   driver->makeColorKeyTexture(GUITextures[4], video::SColor(0,0,0,0));

   //Load texture for Clock Round End Label
   GUITextures[5] = driver->getTexture("../Media/Textures/round_end_msg.bmp");
   driver->makeColorKeyTexture(GUITextures[5], video::SColor(0,0,0,0));

   //load textures for characters
   //Number zero
   CharTextures[0] = driver->getTexture("../Media/Textures/zero.bmp");
   driver->makeColorKeyTexture(CharTextures[0], video::SColor(0,0,0,0));
   //Number one
   CharTextures[1] = driver->getTexture("../Media/Textures/one.bmp");
   driver->makeColorKeyTexture(CharTextures[1], video::SColor(0,0,0,0));
   //Number two
   CharTextures[2] = driver->getTexture("../Media/Textures/two.bmp");
   driver->makeColorKeyTexture(CharTextures[2], video::SColor(0,0,0,0));
   //Number three
   CharTextures[3] = driver->getTexture("../Media/Textures/three.bmp");
   driver->makeColorKeyTexture(CharTextures[3], video::SColor(0,0,0,0));
   //Number four
   CharTextures[4] = driver->getTexture("../Media/Textures/four.bmp");
   driver->makeColorKeyTexture(CharTextures[4], video::SColor(0,0,0,0));
   //Number five
   CharTextures[5] = driver->getTexture("../Media/Textures/five.bmp");
   driver->makeColorKeyTexture(CharTextures[5], video::SColor(0,0,0,0));
   //Number six
   CharTextures[6] = driver->getTexture("../Media/Textures/six.bmp");
   driver->makeColorKeyTexture(CharTextures[6], video::SColor(0,0,0,0));
   //Number seven
   CharTextures[7] = driver->getTexture("../Media/Textures/seven.bmp");
   driver->makeColorKeyTexture(CharTextures[7], video::SColor(0,0,0,0));
   //Number eight
   CharTextures[8] = driver->getTexture("../Media/Textures/eight.bmp");
   driver->makeColorKeyTexture(CharTextures[8], video::SColor(0,0,0,0));
   //Number nine
   CharTextures[9] = driver->getTexture("../Media/Textures/nine.bmp");
   driver->makeColorKeyTexture(CharTextures[9], video::SColor(0,0,0,0));
   //Colon Character
   CharTextures[10] = driver->getTexture("../Media/Textures/colon.bmp");
   driver->makeColorKeyTexture(CharTextures[10], video::SColor(0,0,0,0));

   //Load textures for score keeping
   //label
   ScoreTextures[0] = driver->getTexture("../Media/Textures/scores.bmp");
   driver->makeColorKeyTexture(ScoreTextures[0], video::SColor(0,0,0,0));
   //Player 1
   ScoreTextures[1] = driver->getTexture("../Media/Textures/player1.bmp");
   driver->makeColorKeyTexture(ScoreTextures[1], video::SColor(0,0,0,0));
   //Player 2
   ScoreTextures[2] = driver->getTexture("../Media/Textures/player2.bmp");
   driver->makeColorKeyTexture(ScoreTextures[2], video::SColor(0,0,0,0));
   //Player 3
   ScoreTextures[3] = driver->getTexture("../Media/Textures/player3.bmp");
   driver->makeColorKeyTexture(ScoreTextures[3], video::SColor(0,0,0,0));
   //Player 4
   ScoreTextures[4] = driver->getTexture("../Media/Textures/player4.bmp");
   driver->makeColorKeyTexture(ScoreTextures[4], video::SColor(0,0,0,0));
   //Player 5
   ScoreTextures[5] = driver->getTexture("../Media/Textures/player5.bmp");
   driver->makeColorKeyTexture(ScoreTextures[5], video::SColor(0,0,0,0));
   //Gun Identifier
   ScoreTextures[6] = driver->getTexture("../Media/Textures/gun.bmp");
   driver->makeColorKeyTexture(ScoreTextures[6], video::SColor(0,0,0,0));
   //Number zero
   ScoreTextures[10] = driver->getTexture("../Media/Textures/score_numbers/zero.bmp");
   driver->makeColorKeyTexture(ScoreTextures[10], video::SColor(0,0,0,0));
   //Number one
   ScoreTextures[11] = driver->getTexture("../Media/Textures/score_numbers/one.bmp");
   driver->makeColorKeyTexture(ScoreTextures[11], video::SColor(0,0,0,0));
   //Number two
   ScoreTextures[12] = driver->getTexture("../Media/Textures/score_numbers/two.bmp");
   driver->makeColorKeyTexture(ScoreTextures[12], video::SColor(0,0,0,0));
   //Number three
   ScoreTextures[13] = driver->getTexture("../Media/Textures/score_numbers/three.bmp");
   driver->makeColorKeyTexture(ScoreTextures[13], video::SColor(0,0,0,0));
   //Number four
   ScoreTextures[14] = driver->getTexture("../Media/Textures/score_numbers/four.bmp");
   driver->makeColorKeyTexture(ScoreTextures[14], video::SColor(0,0,0,0));
   //Number five
   ScoreTextures[15] = driver->getTexture("../Media/Textures/score_numbers/five.bmp");
   driver->makeColorKeyTexture(ScoreTextures[15], video::SColor(0,0,0,0));
   //Number six
   ScoreTextures[16] = driver->getTexture("../Media/Textures/score_numbers/six.bmp");
   driver->makeColorKeyTexture(ScoreTextures[16], video::SColor(0,0,0,0));
   //Number seven
   ScoreTextures[17] = driver->getTexture("../Media/Textures/score_numbers/seven.bmp");
   driver->makeColorKeyTexture(ScoreTextures[17], video::SColor(0,0,0,0));
   //Number eight
   ScoreTextures[18] = driver->getTexture("../Media/Textures/score_numbers/eight.bmp");
   driver->makeColorKeyTexture(ScoreTextures[18], video::SColor(0,0,0,0));
   //Number nine
   ScoreTextures[19] = driver->getTexture("../Media/Textures/score_numbers/nine.bmp");
   driver->makeColorKeyTexture(ScoreTextures[19], video::SColor(0,0,0,0));

   //Initialize clock
   updateRoundTimer(0,0,0);
}

void gameHUD::setGunReady(bool ready)
{
	gunTimerReady = ready;
}

bool gameHUD::getGunReady()
{
	return gunTimerReady;
}
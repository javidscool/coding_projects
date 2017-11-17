//do destructors/constructors
//singleton stuff; copy constructor, assignment
//free message memory
//entities interact (animation, kewl eh?);
//replace exit 0's with proper exiting
//todo : figure out what goes in which baseclass
//todo:: double array Method
//todo:: reference zap method
//animating light intensity for the artifact
//spectator/predator/prey cameras
//scene node dropping

//get the gun version of the game
//thievery ut style gameplay mode
//free for all
//better engine version

//totally redo steering behavior stuff
//cover objects and hiding spot nodes
//graph generation stuff

//agent/sensor memory deallocation stuff
//fix the crash on exit bug
//get rid of local constants

//grab the gun scene node so its not drawn twice
//fix last memory leaks
//catch exceptions

//better t key stuff : make the game class BE the input handler? -- no, use message handling -- for now, this is for debugging so not that important

//#define NODE_MESH_GENERATOR
#include <irrlicht.h>
#include <irrklang.h>
#include <vector>
#include <string>
#include <limits>
#include <iostream>
#include <algorithm>
#include "doubleArray.h"
#include "InputHandler.h"
#include "SubjectAgent.h"
#include "Model.h"
#include "ktcGame.h"
#include "MessageHandler.h"
#include "gameHUD.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace irrklang;


std::vector<irr::scene::ISceneNode*> specialWalls;


int main(int, char**){
	IrrlichtDevice *device;

	//create the irrlicht device	//IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(1440,900), 32, false, true, true, InputHandler::getInstance());	device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(800,600), 32, false//shadows	 					      ,true, true, InputHandler::getInstance());
	if(device==NULL)return 1;	//create the irrKlang device	// start the sound engine with default parameters
	ISoundEngine* soundEngine = createIrrKlangDevice();
	//ISound* CurrentPlayingSound = 0;
	//ISoundSource* backgroundMusic = soundEngine->addSoundSourceFromFile("../media/sounds/getout.ogg"); 
	//backgroundMusic->setDefaultVolume(0.3f);

	if (!soundEngine){
		printf("Could not startup irrKlang sound engine\n");
		return 0; // error starting up the engine
	}

	//Play some sound while all this is starting, loop it
	//CurrentPlayingSound = soundEngine->play2D("../media/sounds/getout.ogg", true);
	//soundEngine->play2D(backgroundMusic);

	//set the title of the window
	device->setWindowCaption(L"Quake the Can");
	
	//hide the cursor
	device->getCursorControl()->setVisible(false);

	//get the driver, scene manager, and gui environment objects
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
 
	//Add this back in with the return of the Main GUI
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	driver->setFog(irr::video::SColor(255,25,25,25), true, 0,750, 0, true, true);//set the fog properties


	/***Load the map***/	//load the pk3 file containing the .bsp map file into the engine file system	device->getFileSystem()->addZipFileArchive("../media/map-20kdm2.pk3"); 
	//get the mesh from the map bsp file
	scene::IQ3LevelMesh* mesh = (scene::IQ3LevelMesh*) (smgr->getMesh("maps/20kdm2.bsp"));
 

	//add a scene node for the map
	scene::ISceneNode *mapNode = NULL;//scene node representing the level itself
	mapNode = smgr->addOctTreeSceneNode(

	mesh->getMesh(quake3::E_Q3_MESH_GEOMETRY), 0, -1, 1024);
  
	if(!mesh){throw new std::string("mesh creation error");}
 
	//code i took from an irrlicht tutorial to load in quake 3 shader items
	scene::IMesh * additional_mesh = mesh->getMesh ( quake3::E_Q3_MESH_ITEMS );

	for ( u32 i = 0; i!= additional_mesh->getMeshBufferCount (); ++i ){
		IMeshBuffer *meshBuffer = additional_mesh->getMeshBuffer ( i );
		const video::SMaterial &material = meshBuffer->getMaterial();

		//! The ShaderIndex is stored in the material parameter
		s32 shaderIndex = (s32) material.MaterialTypeParam2;

		//the meshbuffer can be rendered without additional support, or it has no shader
		const quake3::SShader *shader = mesh->getShader ( shaderIndex );

		if( 0 == shader ){throw new std::string("Error loading shaders");}
		mapNode->addChild((smgr->addQuake3SceneNode ( meshBuffer, shader )));
	}
	
	mesh->releaseMesh ( quake3::E_Q3_MESH_ITEMS );

	if(!mapNode)return 1;


	//CPTODO: get rid of local constant
	mapNode->setPosition(core::vector3df(-1300,-144,-1249));
	mapNode->setMaterialFlag(video::EMF_LIGHTING, true);//enable lighting
	mapNode->setMaterialType(irr::video::EMT_LIGHTMAP_LIGHTING_M4);//set the material property of the map to blend the lightmap with dynamic lighting
	smgr->addSkyDomeSceneNode(driver->getTexture("../media/skydome.jpg"),32,32,1.0f,2.0f); //create the skydome

	mapNode->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);//enable fogging on the map node


	//create a triangle selector object for the map for use in creating collisions
	irr::scene::ITriangleSelector*  selector = NULL;
	selector = smgr->createOctTreeTriangleSelector(mesh->getMesh(quake3::E_Q3_MESH_GEOMETRY),//map->getMesh(0), 
												   mapNode,128); 
	if(!selector)return 1;
	mapNode->setTriangleSelector(selector);

	{//block containing the game object

	//create game HUD object
	 gameHUD* display = gameHUD::getInstance();
	 display->setVideoDriver(driver);
	//create the game object


	irr::scene::ISceneNode* x = smgr->addCubeSceneNode();
	x->setPosition(vector3df(-400,-41,-42));
	x->setScale(vector3df(45,1,5));
	x->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));
	x->setMaterialTexture(1,driver->getTexture("../media/wall.jpg"));
	x->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	specialWalls.push_back(x);

	irr::scene::ISceneNode* x2 = smgr->addCubeSceneNode();
	x2->setPosition(vector3df(-400,-41,232));
	x2->setScale(vector3df(45,.9,5));
	x2->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));
	x2->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	specialWalls.push_back(x2);

	irr::scene::ISceneNode* x3 = smgr->addCubeSceneNode();
	x3->setPosition(vector3df(-603,-41,104));
	x3->setScale(vector3df(5,.95,45));
	x3->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));
	x3->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	specialWalls.push_back(x3);


	x3 = smgr->addCubeSceneNode();
	x3->setPosition(vector3df(825,-41,104));
	x3->setScale(vector3df(5,1.25,45));
	x3->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));
	x3->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	specialWalls.push_back(x3);


	x = smgr->addCubeSceneNode();	x->setPosition(vector3df(-400+1000,-41,-42));	x->setScale(vector3df(45,1.05,5));	x->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));	x->setMaterialTexture(1,driver->getTexture("../media/wall.jpg"));	x->setMaterialFlag(video::EMF_FOG_ENABLE, true);	specialWalls.push_back(x);

	x2 = smgr->addCubeSceneNode();	x2->setPosition(vector3df(-400+1000,-41,232));	x2->setScale(vector3df(45,.95,5));	x2->setMaterialTexture(0,driver->getTexture("../media/wall.jpg"));	x2->setMaterialFlag(video::EMF_FOG_ENABLE, true);	specialWalls.push_back(x2);
	ktcGame game(device, selector, display);	/*******************************************************/	/***************GAME UPDATE LOOP************************/	/*******************************************************/	while(device->run()){				//run update on the message handler to send any delayed messges that have passed their time stamp		MsgHandler->update(device->getTimer());
		game.update(device->getTimer());

		//this has been taken out for planning based on menu system 
		guienv->drawAll();
		
		if(InputHandler::getInstance()->EXIT_MESSAGE)
			break;
	}

	}//scope containing the game
	
	//device->drop();
	//soundEngine->drop();
	return 0;
}
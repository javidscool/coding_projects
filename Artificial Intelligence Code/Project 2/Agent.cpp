#define NUMFEELERS 6  //3
#include "Agent.h"
#include <iostream>

#include "InputHandler.h"
#include "MessageHandler.h"

#include "cpMath.h"
#include <cmath>
#include <string>
#include <vector>
#include <limits>

#include "mapGraph.h"
#include <iostream>
using std::cout;

using namespace irr;
using namespace irr::core;
using namespace irr::video;

//std::vector<int>* astarSearch(unsigned int src, unsigned int tgt);
//extern irr::core::vector3df SEEK_POS;


canEntity* Agent::gameCan;
std::vector<Agent*>* Agent::agentList;
std::vector<coverObject*>* Agent::coverObjectList;




GamePlayer* Agent::getSpottedAgent(){

	GamePlayer* tAgt= NULL;

	double sDist = std::numeric_limits<double>::max();
	for(int i = 0; i < GamePlayer::getPlayerList()->size();i++){
	
		GamePlayer* agt = (*GamePlayer::getPlayerList())[i];

		//should be closest non safe
		if(agt->isSafe() || agt->getSceneNode() == this->getSceneNode())continue;
		if(((agt->getSceneNode()->getPosition() - this->getSceneNode()->getPosition()).getLength() < sDist)){
			if( ! agt->getSceneNode()->isVisible())continue; // if the player dies, bug occurs
			sDist = (agt->getPosition() - this->getSceneNode()->getPosition()).getLength();
			tAgt = agt;
		
		}
	
	}



	SPOTTED = tAgt;
	return tAgt;

}

void Agent::setPlayerType(GamePlayer_Type T) {
	type = T;
	if(type == PREDATOR)
		this->GetFSM()->ChangeState(Start::GetInstance());
	else
		this->GetFSM()->ChangeState(Hide::GetInstance());
}



std::list<irr::core::vector3df> Agent::generateDefenseArc(double startAngle, double endAngle, double radius, double nodeCount){
	std::list<irr::core::vector3df> result(nodeCount);

	const irr::core::vector3df& canPos = this->getCan()->getSceneNode()->getPosition();

	double increment = (startAngle - endAngle) / nodeCount;

	for(int i = 0; i < nodeCount; i++){
		double currentAngle = endAngle + increment*i;
		result.push_back( 
			vector3df(0, -canPos.Y + this->getSpawnPoint().Y,0)+radius*vector3df( cos(currentAngle)  ,0,sin(currentAngle) ) + canPos);
		//irr::scene::ISceneNode* a = this->smgr->addSphereSceneNode(5);
		//a->setPosition(result.back());
	}
	
	return result;
}



//this needs to be moved
bool GamePlayer::isSafe(){

	//if i'm it, i'm safe
	if(this->getIt() == this)return true;

extern irr::scene::ISceneManager* smgr;
extern irr::scene::ITriangleSelector*  selector;

//if the distance between me and the person who is it is greater than the fog distance, i'm safe
if( (this->getIt()->getSceneNode()->getPosition() - mynodep->getPosition()).getLength() > 750){
return true;
}

irr::core::line3d<irr::f32> line;

core::vector3df intersection;
core::triangle3df triangle;

line.end = this->getIt()->getSceneNode()->getPosition();
//line.end = //getIt()->getLineOfSight().normalize() * 750;
line.start = mynodep->getPosition();//line.end + line.start;

//line.start = ((line.end - line.start).normalize() * 75) + line.start;

//if the level is in the way between me and IT, I'm ok
if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
//	std::cout<<"Wall in the way\n";
	return true;
}


//if some other obstacle is in the way between me and IT, Im alright
irr::scene::ISceneNode* tnode; 
tnode= smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(line);
if(this->getIt()->getSceneNode() != tnode){

	//std::cout<<"Something else in the way\n";
	return true;

}



//chuckie now hears footsteps
//if it is facing away from me then i'm alright
//irr::core::vector3df vec1 = getIt()->getLineOfSight();
//irr::core::vector3df vec2 = mynodep->getPosition() - getIt()->getSceneNode()->getPosition();
//if(vec1.dotProduct(vec2)< 0)return true;


return false;



if(tnode != mynodep){return true;}
else return false;

}



bool Agent::spotted(Agent* agt){

	if(agt->isSafe())return false;
	return true;
}



void Agent::setSpeed(){
	if(type == PREY){
		MAXSPEED = .15f;
		ACCELRATE = MAXSPEED/4;
	}
	else{
		MAXSPEED = .15f;
		ACCELRATE = MAXSPEED/4;
	}
}

//update method
void Agent::update(const irr::ITimer* timer){


	//if(!isSafe()){
	//	std::cout<<"I'm NOT safe!\n";
	//}return;

	//compute the time interval between now and the last frame for use in time based updating
	irr::u32 ctime = 0;
	TIMEELAPSED = (irr::f32)((ctime = timer->getTime()) - LASTUPDATE);
	LASTUPDATE = ctime;
	
	static double runningAverage = 0;
	static int runs = 0;

	runs += 1;
	runningAverage += TIMEELAPSED;
	if(runs%100 == 0){
		//std::cout<< (runningAverage/ (double) runs)<<"\n";
		runningAverage = runs = 0;
	}

	//update sensors
	updateWallSensor();
	updateProximitySensor();
	updatePieSensor();
	
	//running update() on the state machine
	AgentStateMachine->update(timer);
	
	pl_time.update(timer);
	pl_inv.update(timer);

	if(pl_inv.getTime() >= 0){
		//cout << "I'm invincible now, yay!\n";
	}

}


bool Agent::processMessage(const Message* m){
	return AgentStateMachine->processMessage(m);
}


//ctor
Agent::Agent(Model m, irr::core::vector3df sp, Timer tim, Timer inv, GamePlayer_Type T, irr::core::vector3df p, irr::scene::ISceneManager* mgr, mapGraph* g) : model(m), graph(g), GamePlayer(sp, tim, inv, T){

	//this->graph = NULL;
	mynodep = NULL;

	IT = SPOTTED = NULL;
	this->LAST_OBSTACLE_CORRECTANCE = 0;
	s1d = new WallSensorData(NUMFEELERS,ANGLE);
	pie = new PieSensor(4);

	this->position = p;

	MOVING = false;

	velocity = vector3df(0.0f,0.0f,0.0f);

	//set up state machine
	AgentStateMachine = new StateMachine<Agent>(*this);

	//setting the current state for the agent, which'll be different if the agent is a predator that if the agent is prey
	if(type == PREDATOR){
		std::cout << "I'm a predator and i'm waiting.\n";
		MAXSPEED = .3f;
		//wait 10 secs 
		AgentStateMachine->SetCurrentState(Wait::GetInstance());
		AgentStateMachine->StartCurrentState();
	}

	if(type == PREY){
		std::cout << "I'm prey and i'm waiting.\n";
		MAXSPEED = .15f;
		AgentStateMachine->SetCurrentState(Wait::GetInstance());
		AgentStateMachine->StartCurrentState();
	}

	//if(mgr == NULL){
	//	std::cout<<"NO SCENE MANAGER WTF";}
	//else{
	//	std::cout<<"HAS SCENE MANAGER WHOO";
	//}

	if(mgr){
		mynodep = mgr->addAnimatedMeshSceneNode(m.mesh);
		//std::cout<<"\nADDED MESH SCENE NODE LOL\n";
		mynodep->setPosition(p);
		mynodep->setMaterialTexture(0,m.texture);
		mynodep->setMaterialFlag(video::EMF_LIGHTING, true);
		((irr::scene::IAnimatedMeshSceneNode*)mynodep)->setMD2Animation(scene::EMAT_STAND);
		mynodep->setRotation(irr::core::vector3df(0.0f,(irr::f32)(0.0f),0.0f));
		mynodep->setScale(irr::core::vector3df((irr::f32)m.scale,(irr::f32)m.scale,(irr::f32)m.scale));
		mynodep->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		//mynodep->addShadowVolumeSceneNode();

		//extern IrrlichtDevice* device;
		//if(device){
			//Model SG =  createModel("../media/Marine_shotgun.md2", "../media/Gshotgun.pcx", device, 3.0f);
			//smgr->addAnimatedMeshSceneNode(SG.mesh);
		//}

		//extern IrrlichtDevice* device;
		//Model CHUCKIERAIL = createModel("../media/w_railgun.md2","../media/w_railgun.pcx",device,1.0f);

		//mynodep->addChild( mgr->addAnimatedMeshSceneNode(CHUCKIERAIL.mesh));
	}

	orientation = //360.0f - 
	0.0f;

	LASTUPDATE = 0;

	smgr= mgr;
	
	
	irr::scene::IBillboardTextSceneNode* a = flavorText = 
 
    mgr->addBillboardTextSceneNode(0,stringw("I'm it!").c_str() );
		
	a->setPosition(vector3df(0.0f, 25.0f,0.0f));
	//a->setPosition(mynodep->getPosition());
	mynodep->addChild(a);
	a->setMaterialFlag(video::EMF_ZBUFFER,true);
	a->setMaterialFlag(video::EMF_LIGHTING, false);
	a->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	a->setSize(core::dimension2d<f32>(20.0f, 20.0f));
	a->setVisible(true);
	//mynodep->addChild(smgr->addLightSceneNode(0,vector3df(-10,25*model.scale,-10),video::SColor(255,255,255,255),1000));
	currentSeekTarget = mynodep->getPosition();
	previousSeekTarget = mynodep->getPosition();
}


Agent::~Agent(){
	//deleting the state machine
	delete AgentStateMachine;
}




void Agent::createCollisionAnimator(irr::scene::ITriangleSelector* sel ,irr::scene::ISceneManager* mgr){
	
	selector = sel;
	scene::ISceneNodeAnimator *nodeAnimator;
	nodeAnimator = mgr->createCollisionResponseAnimator(selector, mynodep, core::vector3df((irr::f32)(30.0f*model.scale),(irr::f32)(25.0f*model.scale),(irr::f32)(30.0f*model.scale)),//collision volume radii
														core::vector3df((irr::f32)0.0f,(irr::f32)-10.0f,(irr::f32)0.0f),//gravity 
														model.mesh->getBoundingBox().getCenter() ); //collision volume position
 
	mynodep->addAnimator(nodeAnimator);
	//mynodep->setScale(core::vector3df(1.75f,1.75f,1.75f));
	nodeAnimator->drop();

	smgr = mgr;
}


//This finds the relative distance between two agents
double Agent::agentProximity(Agent *nearAgent){
	
	//Calculate magnitude
	double x,z ;
	x = nearAgent->position.X - this->position.X ;
	z = nearAgent->position.Z - this->position.Z ;
	return hypo(x,z);	
}

//This finds the relative bearing between X axis and agent
double Agent::agentBearing(Agent *nearAgent){
	
	//Some variables
	double magAdj, magHypo;
	double diffX, diffZ;
	
	//Quadrant of nearAgent
	int quad = -1;

	//nearAgent angle for return
	double nearAngle = -1;

	//math
	diffX= nearAgent->position.X - this->position.X;
	diffZ= nearAgent->position.Z - this->position.Z;
	magHypo = hypo(diffX,diffZ);
	magAdj = abs(diffX);

	//Determine which quadrant the nearAgent is in
	if(diffX > 0)
	{
		//First quad check
		if(diffZ > 0)
			quad = 1;
		//Fourth quad check
		else if(diffZ < 0)
			quad = 4;
		//Angle zero
		else if(diffZ == 0)
			nearAngle = 0;
	}
	else if(diffX < 0)
	{
		//Second quad check
		if(diffZ > 0)
			quad = 2;
		//Third quad check
		else if(diffZ < 0)
			quad = 3;
		//Angle 180
		else if(diffZ == 0)
			nearAngle = 180;

	}
	else if(diffX == 0)
	{
		//Angle 90
		if(diffZ > 0)
			nearAngle = 90;
		//Angle 270
		else if(diffZ < 0)
			nearAngle = 270;
	}

	//If angle has been determined, don't calculate
	if(nearAngle < 0)
	{
		//calculate cos of the triangle
		nearAngle = acos(magAdj/magHypo);
		
		//convert from radians to degrees
		nearAngle = radiansToDegrees(nearAngle);
	}

	//Compensates depending on the quadrant nearAgent is located in
	switch(quad){
		case 1:
			nearAngle += 0;
			break;
		case 2:
			nearAngle = 180 - nearAngle;
			break;
		case 3:
			nearAngle += 180;
			break;
		case 4:
			nearAngle = 360 - nearAngle;
			break;
		default:
			nearAngle += 0;
			break;
	}


	return nearAngle;

}


//This is Sensor 2, updates list of nearby agents
void Agent::proximitySensor(double sensorRange)
{	
	//variables
	double alpha, theta;

	//get the agentList size
	int aListSize = Agent::agentList->size();

	//Temp sensor data storage, this gets added to the list
	ProximitySensorData *temp ;
	temp = new ProximitySensorData[aListSize];

	//Traverse entire list
	for(int x=0 ; x< aListSize ; x++)
	{
		//Ignore entry in list that is self
		if((*agentList)[x]!= this)
		{ 
			//Get relative distance between agents and store in temp
			temp[x].relDistance = (*agentList)[x]->agentProximity(this);

			//If agents are within range, then add to the list
			if(temp[x].relDistance <= sensorRange)
			{
				//Set ID to pointer
				temp[x].agentID = (int)(*agentList)[x];

				//Get relative bearing and store in temp
				alpha = this->agentBearing((*agentList)[x]);
				theta = this->orientation ;
				//Calculate angle between
				if( abs(alpha-theta) < (360-abs(alpha-theta)))
					temp[x].relHeading = alpha-theta;
				else{
					//if the alternate angle has a smaller magnitude 
					//we take it instead
					
					//assumed that this will return a positive value less than or equal to 
					//360
					temp[x].relHeading = 360 - abs(alpha - theta);
					
					//if the original angle was less than zero then the alternate is positive
					if(alpha-theta <0.0);
					//otherwise we know that the alternative angle is negative
					else{temp[x].relHeading*=-1;}

				}
				//Add temp to proxSenseList for return
				this->s2d.push_back(&temp[x]);
			}
		}
	}
}

void Agent::PieDetect(){
	//irr::core::vector3df self_normal = irr::core::vector3df(cos(degreesToRadians(orientation)), 0, sin(degreesToRadians(orientation))); 
	for(unsigned int i = 0; i < Agent::agentList->size(); i++){
		if( (*agentList)[i] == this) continue;
		double distance = Agent::agentProximity( (*agentList)[i] );
		if(distance <= pie->range){
			//this is the magnitude of the agent vector AND the distance between myself and the agent 
			//double mag = sqrt( (abc[i]->getPosition().X * abc[i]->getPosition().X) + (abc[i]->getPosition().Y * abc[i]->getPosition().Y) + (abc[i]->getPosition().Z * abc[i]->getPosition().Z) );
			//irr::core::vector3df agent_normal = irr::core::vector3df( (abc[i]->getPosition().X / mag), (abc[i]->getPosition().Y / mag), (abc[i]->getPosition().Z / mag) ); 
			double self_angle = this->orientation;

			//Modify based on increasing orientation
			double agent_angle = Agent::agentBearing((*agentList)[i]);
			double ang_between_players;
			if ( (agent_angle - self_angle) < 0){
				ang_between_players = 360.0 - fabs(agent_angle - self_angle);
			}
			else{
				ang_between_players = agent_angle - self_angle;
			}
			
			double final_angle;
			
			final_angle = ang_between_players + pie->offset; 
			if(final_angle > 360) final_angle-=360;

			int place_in_bucket = int( final_angle / fabs(pie->angle) );
			
			if (place_in_bucket > ( (pie->num_slices * 2) -1) ) exit(0);
			pie->areas[place_in_bucket]++;
		}
	}
}

void Agent::showPieSensor(){
	using std::cout;
	using std::endl;
	//cout << "Going counterclockwise starting from the front.\n";
	//cout << "[ ";
	//for(int i = 0; i < (pie->num_slices * 2); i++)
	//	cout << pie->areas[i] << " ";
	//cout << "]" << endl;
}

std::string Agent::WallSensorToString(){
	
	std::string s("Wall Feelers:\n");
	
	double  baseAngle = orientation - s1d->getAngle()/2.0;
	double  increment = s1d->getAngle() / s1d->getNumFeelers();


	for(int i = 0; i < s1d->getNumFeelers(); i++){
		
		double angle = i * increment + baseAngle;

		char str[100];
		sprintf(str,"%f", angle);
		s+= std::string(str);
		//s +="Angle:"+angle;
		s+="\tDistance-Squared:";
		sprintf(str,"%f",s1d->feelerDistances[i]);
		s+= std::string(str);
	}
	
	return s;
}



void Agent::drawPieSlices(irr::video::IVideoDriver * driver){
	
	SMaterial m; 
	m.Lighting=false; 
	m.ZBuffer = 1;
	driver->setMaterial(m); 	
	//SColor col;


	driver->setTransform(video::ETS_WORLD, mynodep->getAbsoluteTransformation());

	//blue is velocity
	driver->draw3DLine(vector3df(0,0,0), vector3df(500,0,0), SColor(255,0,0,255));

	//red is line to targets

	//irr::core::matrix4 abc = irr::core::IdentityMatrix;
	//const float dat[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, currentSeekTarget.X, currentSeekTarget.Y, currentSeekTarget.Z, 1};
	//abc.setM(dat);
	driver->setTransform(video::ETS_WORLD, graph->SCENE_NODE_VECTOR[ graph->getClosestNode(previousSeekTarget)]->getAbsoluteTransformation() );
	driver->draw3DLine( vector3df(0,0,0), this->currentSeekTarget-this->previousSeekTarget, SColor(255, 0, 255,0));
   
   
  

	//green line is path
	//driver->draw3DLine(this->previousSeekTarget - mynodep->getPosition(), this->currentSeekTarget - mynodep->getPosition(), SColor(255, 0, 255,0));


	/*	
	irr::f32 angle = (irr::f32)pie->offset;
	for(int i = 0; i < pie->num_slices*2;i++){
		driver->draw3DLine(
				vector3df(0,0,0), 
				vector3df(
				(irr::f32)(250*cos( degreesToRadians(angle) )),
							0,
							(irr::f32)(250*sin( degreesToRadians(angle) ))), 
							SColor(255,255,255,255));
		angle +=(irr::f32)pie->angle;
   
		//core::matrix4()); 
		//driver->draw3DLine(vector3df(0,0,0), vector3df(0,0,250));
		//driver->draw3DLine(mynodep->getPosition(), mynodep->getPosition()+5000*vector3df(cos(0.0f), mynodep->getAbsolutePosition().Y, sin(0.0f)),video::SColor(255,255,255,255));
	}*/

}






void Agent::createPatrolRoute(mapGraph* mg){
	
	pathList.clear();
	extern irr::scene::ISceneManager* smgr;
	mapGraph* minspanningtree = mg->minimumSpanningTree(0);
	//std::cout<<"got the tree\n";

	if(!mynodep)std::cout<<"no mynodep\n";
		if(!mg)std::cout<<"no mg\n";
			if(!minspanningtree)std::cout<<"no tree\n";
				if(!smgr)std::cout<<"no smgr\n";
					if(!selector)std::cout<<"no selector\n";





	std::vector<int>* result = minspanningtree->depthFirstSearch(mg->getClosestNodeUnobstructedSpannable(mynodep->getPosition(),smgr, selector));
	//delete minspanningtree;
	//minspanningtree= 0;
if(!result)return;

	if(result->size()){
		//pathList.resize(mg->NODE_VECTOR.size());
		for(unsigned int i = 0; i < result->size(); i++){
			pathList.push_back( mg->nodePosition((*result)[i]));
		}
	//pathList.push_back(fin);


	currentSeekTarget = pathList.front();
	previousSeekTarget = mynodep->getPosition();
	}
	else{
		this->pathList.push_back(mynodep->getPosition());
		this->velocity = vector3df(0,0,0);
	}
	
	for(int i = 0; i < result->size(); i++){
		std::cout<<(*result)[i]<<" ";
	}


	/*
	std::list<irr::core::vector3df>::const_iterator iter = pathList.begin();
	for(int i = 0; i < pathList.size()-1; i++){
		core::line3d<f32> line;
		core::vector3df intersection;
		core::triangle3df triangle;
		line.start = *iter;
		iter++;
		line.end = *iter;

		if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
			std::cout<<"WTF SOMEHOW THE PATH IS WRONG\n";///exit(0);
		std::cout<<"From node:"<< mg->getClosestNode(line.start)<<"To Node:"<<mg->getClosestNode(line.end) << std::endl;			
		}
	}
	
	std::cout<<std::endl;
	*/

	delete result;
}


//this function generates a list of waypoints to seek to a target location
void Agent::newTargetLocation(irr::core::vector3df fin){
	
	mapGraph* mg = this->graph;
	//extern std::vector<irr::core::vector3df> NODE_VECTOR;
	
	pathList.clear();


	//get the unobstructed node closest to the target location
	int sNode2 = mg->getClosestNodeUnobstructed(fin,smgr, selector);
	//get the unobstructed node closest to the agent
	int sNode1 = mg->getClosestNodeUnobstructed(mynodep->getPosition(), smgr,selector);

	std::vector<int>* result = mg->astarSearch(sNode1, sNode2);

	if(result->size()){
		for(unsigned int i = 0; i < result->size(); i++){
			pathList.push_front( mg->nodePosition( (*result)[i]));
		}
		pathList.push_back(fin);
	}
	else{
		velocity = vector3df(0,0,0);
		pathList.push_back(mynodep->getPosition());
	}

	currentSeekTarget = pathList.front();
	previousSeekTarget = mynodep->getPosition();
	printf("%d %d\n", sNode1, sNode2);
	
	//agent2.createPatrolRoute(&graph);
	//mg->selector = selector;

	//line.start = NODE_VECTOR[

	//if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
		//std::cout<<"WTF SOMEHOW THE PATH IS WRONG\n";///exit(0);
	//}

	delete result;
}

//this function generates a list of waypoints to seek to a target location
void Agent::newTargetLocationSpannablePath(irr::core::vector3df fin){
	
	mapGraph* mg = this->graph;
	//extern std::vector<irr::core::vector3df> NODE_VECTOR;
	
	pathList.clear();


	//get the unobstructed node closest to the target location
	int sNode2 = mg->getClosestNodeUnobstructedSpannable(fin,smgr, selector);
	//get the unobstructed node closest to the agent
	int sNode1 = mg->getClosestNodeUnobstructedSpannable(mynodep->getPosition(), smgr,selector);

	//std::cout<<"from "<<sNode1<<" to "<<sNode2<<" \n";
	
	std::vector<int>* result = mg->astarSearch(sNode1, sNode2);

	std::cout<<"astar path is\n";
	if(result->size()){
		for(unsigned int i = 0; i < result->size(); i++){
			pathList.push_front( mg->nodePosition( (*result)[i]));
			std::cout<<mg->getClosestNode(mg->nodePosition( (*result)[i]))<<" ";
		}
		std::cout<<"\n";
		pathList.push_back(fin);
	}
	else{
		velocity = vector3df(0,0,0);
		pathList.push_back(mynodep->getPosition());
	}
	
	currentSeekTarget = pathList.front();
	previousSeekTarget = mynodep->getPosition();
	printf("%d %d\n", sNode1, sNode2);
	
	//agent2.createPatrolRoute(&graph);
	//mg->selector = selector;

	//line.start = NODE_VECTOR[

	//if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
	//std::cout<<"WTF SOMEHOW THE PATH IS WRONG\n";///exit(0);
	//}

	delete result;
}

//if the agent needs to correct its path: in the event it gets lost, eg, falls off a bridge, misses a doorway, etc, a-star to the currentSeekTarget and prepend the path to the pathlist
void Agent::correctPath(){
	
	std::cout<<"correcting path\n";
	int src = this->graph->getClosestNodeUnobstructed(this->getPosition(),smgr,selector);
	int tgt = this->graph->getClosestNodeUnobstructed(currentSeekTarget,smgr,selector);



	//CPNOTE::WHAT? Shouldnt' this be tgt= stuff?  Why did i do that?  It's day before release and I don't want to go changing things if it seems to be working...
	//when the agent is getting stuck on the torch, they're closest to the node behind the torch, so source is equal to target and no path correction is getting done
	//this makes the agent backtrack and try again, but it will prevent them from getting stuck permanently
	if(src == tgt){
		//std::cout<<"Source equal to target in path correction, doing path correction correction\n";
		src = this->graph->getClosestNodeUnobstructed(this->previousSeekTarget,smgr,selector);
	}

	std::vector<int>* result = this->graph->astarSearch( src, tgt);
	pathList.push_front(this->currentSeekTarget);
	if(result->size()){
		for(unsigned int i = 0; i < result->size(); i++){
			pathList.push_front( this->graph->nodePosition( (*result)[i]));
		}
	}

	currentSeekTarget = pathList.front();
	previousSeekTarget = mynodep->getPosition();
	

	//std::cout<<"PATH CORRECTION IN PLACE\n";
	delete result;
}



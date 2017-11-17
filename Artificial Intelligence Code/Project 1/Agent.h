#ifndef AGENT
#define AGENT

//vsync bug
//dropped behavior
//quick hacks

#include "irrlicht.h"
#include "GameEntity.h"
#include "Sensors.h"
#include "Model.h"
#include <string>
#include <list>
#include "mapGraph.h"
#include "StateMachine.h"
#include "AgentStates.h"
#include "physicsObject.h"
#include "Timer.h"
#include "GamePlayer.h"
#include "coverObject.h"
#include <vector>

const double mass = 25; //was 100 // was 25const double RADIUS = 100;//50;//25;//was 100//const double ANGLE = 45;const double TIMEMULTIPLIER = 2.0f;const double ANGLE = 45.0f;

class Agent : public GamePlayer{private:
	irr::f32 TIMEELAPSED;	double LAST_OBSTACLE_CORRECTANCE;	double pathStartTime;	double expectedArrivalTime;

	double ACCELRATE;	physicsObject* IT;	physicsObject* SPOTTED;	coverObject* myCoverObject;		irr::u32 LASTUPDATE;	
	
	//an object of the state machine that the agent uses to implement an FSM
	StateMachine<Agent> * AgentStateMachine;

	//pathfinding information
	irr::core::vector3df currentSeekTarget;
	std::list<irr::core::vector3df> pathList;
	std::list<irr::core::vector3df> pathListAvd;
	irr::core::vector3df previousSeekTarget;
	mapGraph* graph;


	//sensor data
	WallSensorData *s1d;
	std::vector<ProximitySensorData*> s2d;
	PieSensor *pie;

	//list of all the agents other agents can "see"
	static std::vector<Agent*>* agentList;	static std::vector<coverObject*>* coverObjectList;		//data structure representing the 3d model	Model model;

	//engine related stuff
	irr::scene::ITriangleSelector* selector;

	void correctPath();


public:
	
	bool there;
    bool MOVING;

	scene::ISceneManager* smgr;	virtual void setSpeed();	irr::core::vector3df getCurrentSeekTarget(){return currentSeekTarget;}	void setIt(physicsObject* p){IT = p;}	void setSpotted(physicsObject* p){SPOTTED = p;}
	physicsObject* getIt(){return IT;}
	physicsObject* getSpottedAgent(){return SPOTTED;}

	irr::f32  getUpdateTimeIncrement(){return this->TIMEELAPSED;}

	//pathfinding functions
	irr::core::vector3df seek(irr::core::vector3df);
	irr::core::vector3df wallAvoidance();
	irr::core::vector3df followPath(const irr::ITimer* timer);
	irr::core::vector3df flee(irr::core::vector3df);
	void newTargetLocation(irr::core::vector3df);
	void newTargetLocationSpannablePath(irr::core::vector3df);
	void createPatrolRoute(mapGraph* mg);

	irr::core::vector3df pursue(physicsObject* tgt);
	irr::core::vector3df avoid(physicsObject* tgt);

	//gameEntity functionality
	virtual void update(const irr::ITimer*);
	virtual bool processMessage(const Message*);

	//model getter/setter
	inline void setModel(Model m){model = m;}
	inline Model& getModel(){return model;} 

	//agentList getter/setter
	inline static void setAgentList(std::vector<Agent*>* abc){agentList = abc;}
	inline static std::vector<Agent*>* getAgentList(){return agentList;}

	//cover node list getter/setter	inline static void setCoverObjectList(std::vector<coverObject*>* abc){coverObjectList = abc;}	inline static std::vector<coverObject*>* getCoverObjectList(){return coverObjectList;}	//seek target getter/setter
	inline irr::core::vector3df getSeekTarget(){return currentSeekTarget;}
	inline void setSeekTarget(irr::core::vector3df pl){ currentSeekTarget = pl;}
	inline irr::core::vector3df getPreviousSeekTarget(){return previousSeekTarget;}
	
	//pathlist getter/setter
	inline std::list<irr::core::vector3df>& getPathList(){return pathList;}
	inline void setPathList(std::list<irr::core::vector3df> pl){
		pathList.clear();
		pathList = pl;
		pathList.push_front(mynodep->getPosition());
		currentSeekTarget = pathList.front();
		previousSeekTarget = mynodep->getPosition();
		this->velocity = vector3df(0,0,0);
	}

	//fsm getter
	inline StateMachine<Agent> * GetFSM() const{ return AgentStateMachine; }
	
	//rendering function for sensor data
	void drawPieSlices(irr::video::IVideoDriver*);


	Agent(Model m, irr::core::vector3df sp, Timer tim, Timer inv, GamePlayer_Type T, irr::core::vector3df p = irr::core::vector3df(0.0f,0.0f,0.0f),irr::scene::ISceneManager* mgr = NULL, mapGraph* g=0);
	virtual ~Agent();

	//i don't think any of this is mine
	//double hypo(double opp, double adj);
	double agentProximity(Agent *nearAgent);
	double agentBearing(Agent *nearAgent);
	void proximitySensor(double sensorRange);
	void PieDetect();
	void showPieSensor();


	//creates collision detection on the agent with the world
	void createCollisionAnimator(irr::scene::ITriangleSelector* selector ,irr::scene::ISceneManager* mgr);

	//sensor update functions
	virtual void updateWallSensor();
	void updateProximitySensor();
	void updatePieSensor();

	//outputs wall sensor data as a human-readable string
	std::string WallSensorToString();

	//graph getter/setter
	inline mapGraph* getGraph(){return graph;}
	inline void setGraph(mapGraph* g){graph=g;}

	irr::core::vector3df Agent::hide();

	void walk(irr::core::vector3df accel);
	void walk(){ walk(irr::core::vector3df(0,0,0)); }

	coverObject* getMyCoverObject(){return myCoverObject;}	void setMyCoverObject(coverObject* c){myCoverObject = c;}};
#endif
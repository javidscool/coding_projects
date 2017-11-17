//interrupt handle
//torches
//circles
//angle feelers up?
//corners

#ifndef SENSORS
#define SENSORS

#include<string>
#include<iostream>
#include<vector>

#include "irrlicht.h"
class Agent;

//Wall feeler sensor
class WallSensorData{
private:
	int numFeelers;
	double angle;

public:
	float maxRange;
		WallSensorData();	WallSensorData(int nf,double a, float mr = 50			//float mr = 5000.0f			);
	~WallSensorData();

	int getNumFeelers();
	double getAngle();

	float* feelerDistances;
	irr::core::triangle3df* triangle;
};

class ProximitySensorData{
public:
	//Agent Identification Number
	int agentID;

	//Relative distance to agent
	double relDistance;

	//Relative heading to agent
	double relHeading;
		
	ProximitySensorData();
	ProximitySensorData(int id, double rD, double rH);
	~ProximitySensorData();
};

class PieSensor{
public:
	//Agent *pt;
	int num_slices;
	double range;
	double orientation;
	double angle;
	double offset;
	int *areas;
	
	double degreesToRadians(double degrees){return 2*3.14159*degrees/360;}
	PieSensor();
	PieSensor(int);
	~PieSensor();
	void clear();
	void Detect(std::vector<Agent*> *);
	void showPieSensor();
};

#endif
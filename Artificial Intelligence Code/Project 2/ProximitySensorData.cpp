#include "Sensors.h"


ProximitySensorData::ProximitySensorData(){
	//default
}

ProximitySensorData::ProximitySensorData(int id, double rD, double rH){
	//Assign values to ProximitySensorData object
	agentID = id;
	relDistance = rD;
	relHeading = rH;
}


ProximitySensorData::~ProximitySensorData(){
	//destructor
}

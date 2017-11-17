#include "Sensors.h"

WallSensorData::WallSensorData(){
	//default
}

WallSensorData::WallSensorData(int nf,double a, float mr){
	maxRange = mr;
	angle = a;
	numFeelers = nf;
	feelerDistances = new float[nf];
	triangle = new irr::core::triangle3df[nf];
}

WallSensorData::~WallSensorData(){
	//destructor
}

int WallSensorData::getNumFeelers(){
	return numFeelers;
}


double WallSensorData::getAngle(){
	return angle;
}

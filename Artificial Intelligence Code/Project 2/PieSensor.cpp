#include "Sensors.h"

PieSensor::PieSensor(){
	//default
}

PieSensor::PieSensor(int slice){
	num_slices = slice;
	range = 5000;
	angle = 360.0 / (num_slices * 2);
	offset = 360.0 / (num_slices * 4);
	orientation = 0;
	areas = new int[num_slices * 2];
	clear();
}

void PieSensor::clear(){
	for(int i = 0; i < (num_slices * 2); i++)
		areas[i] = 0;
}

PieSensor::~PieSensor(){
	delete [] areas;
}
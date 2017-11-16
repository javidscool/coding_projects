#include "TravMap.h"

//--------------------Create Cities-------------------------
//	Evenly spaces the cities on the perimeter of a wheel and
//  puts a vector of their coordinates in vecCityCoords
//----------------------------------------------------------
void TravMap::CreateCircularCities(){
	//first calculate the radius of spread and the origin

	//margin so that cities don't land on edge og map
	const int margin = 50;

	double radius;

	int idCounter = 0;

	if(mapHeight < mapWidth){
		radius = (mapHeight/2) - margin;
	}

	else{
		radius = (mapWidth/2) - margin;
	}

	Coord origin(-100, mapWidth / 2, mapHeight / 2);

	//calculate angle division between adjacent cities
	double segSize = 2 * pi/ numCities;

	double angle = 0.0;

	while(angle < 2 * pi){
		Coord thisCity;

		thisCity.x = radius * sin(angle) + origin.x;
		thisCity.y = radius * cos(angle) + origin.y;

		thisCity.id = idCounter;

		vecCityCoords.push_back(thisCity);

		angle += segSize;

		idCounter++;
	}
}

//-------------------------------BestPossibleFitness---------------------------------
//	calculates the distance of the optimum route. We use this so we
//	can tell when to stop each run
//----------------------------------------------------------------------
void TravMap::CalculateBestPossibleRoute()
{
	bestPossibleRoute = 0;
	
	for (int city=0; city < vecCityCoords.size()-1; city++){
		bestPossibleRoute += CalculateA_To_B(vecCityCoords[city], vecCityCoords[city+1]);
		
		//add in a small amount to cover any precision errors we may have made
		bestPossibleRoute += EPSILON;
	}
	
	//add in the distance from the last to the first
	bestPossibleRoute += CalculateA_To_B(vecCityCoords[vecCityCoords.size()-1], vecCityCoords[0]);
}


//---------------------Calculate Distance----------------------
//  this is basically the distance formula
//	it returns the distance between the two given cities
//-------------------------------------------------------------
double TravMap::CalculateA_To_B(const Coord &city1, const Coord &city2)
{
	double xDist = city1.x - city2.x;
	double yDist = city1.y - city2.y;
	
	return sqrt(xDist*xDist + yDist*yDist);
}


//-----------------Calculate Fitness ---------------------------
//	calculates the total distance the salesman must travel for
//	the given order of cities.  this is basically our fitness function
//--------------------------------------------------------------
double TravMap::GetTourLength(const vector<int> &route)
{
	double totalDistance = 0;
	
	for (int i = 0; i < route.size()-1; i++){
		int city1 = route[i];
		int city2 = route[i+1];
		
		totalDistance += CalculateRealDist(city1, city2); 
	}
	
	//since this is a closed loop so we need to add in the distance 
	//from the last city visited back to the first
	int last  = route[route.size()-1];
	int first = route[0];
	
	totalDistance += CalculateRealDist(last, first);
	
	return totalDistance;
}

double TravMap::CalculateRealDist(int num1, int num2){
	Coord start, end;

	for(int i = 0; i < numCities; i++){
		if(num1 == vecCityCoords[i].id)
			start = vecCityCoords[i];
		else if(num2 == vecCityCoords[i].id)
			end = vecCityCoords[i];

		if(start.id != -1 && end.id != -1) break;
	}
	
	return CalculateA_To_B(start, end);

}

//this switch statement tells us what kind of change to make
void TravMap::Change(ChangeType type){
	switch(type){
		case SMALL:
			SmallChange();
			break;
		case BIG:
			BigChange();
			break;
		case OSCBIG:
			OscillationBig();
			break;
		case OSCSMALL:
			OscillationSmall();
			break;
	}
}

//executing a small change
void TravMap::SmallChange(){
	cout << "Small Change\n";

	//pick start and ending cities to switch
	int startSwitch = RandInt(0, numCities - 1);
	int endSwitch = (startSwitch + 1) % numCities;

	bool foundStart = false, foundEnd = false;

	for(int i = 0; i < vecCityCoords.size(); i++){
		if(vecCityCoords[i].id == startSwitch){
			vecCityCoords[i].id = endSwitch;
			foundStart = true;
		}
		else if(vecCityCoords[i].id == endSwitch){
			vecCityCoords[i].id = startSwitch;
			foundEnd = true;
		}

		if(foundStart && foundEnd) break;
	}
}

//executing a big change
void TravMap::BigChange(){
	cout << "Big Change\n";

	//pick start and ending cities to switch
	int startSwitch = RandInt(0, numCities - 1);
	int endSwitch = (startSwitch + 5) % numCities;

	bool foundStart = false, foundEnd = false;

	for(int i = 0; i < vecCityCoords.size(); i++){
		if(vecCityCoords[i].id == startSwitch){
			vecCityCoords[i].id = endSwitch;
			foundStart = true;
		}
		else if(vecCityCoords[i].id == endSwitch){
			vecCityCoords[i].id = startSwitch;
			foundEnd = true;
		}

		if(foundStart && foundEnd) break;
	}
}

//getting the two cities to oscillate
void TravMap::GetOscCoords(){
	int oneCoord, twoCoord;
	bool oneFound = false, twoFound = false;

	//we use numCities-1 because we want ints numbered from zero
	oneCoord = RandInt(0, numCities-1);

	//we use numCities-1 because we want ints numbered from zero
	twoCoord = RandInt(0, numCities-1);

	while(oneCoord == twoCoord)
	{
		//give me another random number if the previous one is already in the list
		twoCoord = RandInt(0, numCities-1);
	}

	for(int i = 0; i < numCities; i++){
		if(oneCoord == vecCityCoords[i].id){
			startOsc = vecCityCoords[i];
			oneFound = true;
		}
		else if(twoCoord == vecCityCoords[i].id){
			endOsc = vecCityCoords[i];
			twoFound = true;
		}

		if(oneFound && twoFound) break;
	}

}

//executing a big oscillation
void TravMap::OscillationBig(){
	cout << "Big Oscillation\n";

	bool foundStart = false, foundEnd = false;

	for(int i = 0; i < vecCityCoords.size(); i++){
		if(vecCityCoords[i].id == startOsc.id){
			vecCityCoords[i].id = endOsc.id;
			foundStart = true;
		}
		else if(vecCityCoords[i].id == endOsc.id){
			vecCityCoords[i].id = startOsc.id;
			foundEnd = true;
		}

		if(foundStart && foundEnd) break;
	}
}

//executing a small oscillation
void TravMap::OscillationSmall(){
	cout << "Small Oscillation\n";

	bool foundStart = false, foundEnd = false;

	for(int i = 0; i < vecCityCoords.size(); i++){
		if(vecCityCoords[i].id == startOsc.id){
			vecCityCoords[i].id = endOsc.id;
			foundStart = true;
		}
		else if(vecCityCoords[i].id == endOsc.id){
			vecCityCoords[i].id = startOsc.id;
			foundEnd = true;
		}

		if(foundStart && foundEnd) break;
	}

}

void TravMap::Reset(){
	for(int i = 0; i < numCities; i++){
		vecCityCoords[i].id = i;
	}
}








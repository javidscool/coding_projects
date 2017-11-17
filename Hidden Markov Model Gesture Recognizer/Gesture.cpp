#include "Gesture.h"
#include <assert.h>

using namespace std;

//getter methods
Gesture_Name Gesture::GetGestName(){
	return gestName;
}


vector<ObservationSeq> & Gesture::GetObsSamples(){
	return observSamples;
}


//ctors
//the number of means for the codebook is the same as the number of observations
//the last argument is the minimum number of elements a codebook cluster can have
Gesture::Gesture(Gesture_Name gName, const unsigned int n, const unsigned int m) : gestHMM(n, m){
	gestName = gName;
}

//the number of means for the codebook is the same as the number of observations
//the last argument is the minimum number of elements a codebook cluster can have
Gesture::Gesture(Gesture_Name gName, vector<ObservationSeq> obSamples, const unsigned int n, const unsigned int m) : gestHMM(n, m){
	gestName = gName;
	observSamples = obSamples;
}

//function to add an observation sequence to the sample space
void Gesture::AddObsSeq(vector<SkeletonInfo> & siList){
	
	//checking that we even have skeleton information
	assert(siList.size() > 0);

	//create the observation sequence
	ObservationSeq myObservation(siList);

	//add the observation sequence to the list of observation sequences
	observSamples.push_back(myObservation);

}


//function that saves the gesture
void Gesture::Save(ofstream & fout){
	//saving the HMM of this gesture
	gestHMM.Save(fout);
}

//function that loads the gesture
void Gesture::Load(ifstream & fin){
	//loading the HMM of this gesture
	gestHMM.Load(fin);
}
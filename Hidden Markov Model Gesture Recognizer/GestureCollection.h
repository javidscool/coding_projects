#ifndef GESTURE_COLLECTION
#define GESTURE_COLLECTION

#include <ctime>
#include <iostream>
#include <vector>

#include "Gesture.h"
#include "CodeBook.h"
#include "Menu.h"

#include "KinectHandler.h"

using namespace std;

//this struct holds the result of the gesture
//it contains the timestamp of the gesture, which is when the gesture was classified,
//the logP probability (because we have to use scaling) and the Gesture_Name enum name
struct GestureResult{
	Gesture_Name resultName;

	time_t timeStamp;

	double logProb;

	GestureResult(Gesture_Name resName, time_t tStamp, double lProb){
		resultName = resName;
		timeStamp = tStamp;
		logProb = lProb;
	}
};

//this class will contain all of our gestures and will have the necessary functionality to classify a gesture given 
class GestureCollection{

public:
	//ctor
	GestureCollection();

	//this function classifies our observation sequence AFTER we've trained our HMMs
	GestureResult ClassifyGesture(ObservationSeq & O);

	//this list contains a list of all of our gestures
	vector<Gesture> gestCollection;

	//function to build the codebook
	void BuildTheCodeBook();

	//function that saves everything important so we can do classification later
	void Save();

	//function that loads everything important so we can do classification
	void Load();

	//function to check if the kinect was initialized properly
	bool GetKinectError();


	//our main menu that controls the whole program and brings together the kinect and the HMM
	Menu menu;


	//the KinectHandler that is in charge of all the kinect shit

	//call GetSuccessfulInit after you create your kinect handler
	KinectHandler kinectHandler;


private:
	//the codebook, we have ONE codebook for ALL gestures
	//**************************************************************************************************
	//***we must get all the observation sequences for this each gesture BEFORE we build the codebook***
	//**************************************************************************************************
	CodeBook codeBook;


	bool kinectError;

};

#endif
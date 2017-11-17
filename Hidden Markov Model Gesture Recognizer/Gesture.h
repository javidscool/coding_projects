#ifndef GESTURE_H
#define GESTURE_H

#include <iostream>
#include <fstream>
#include <vector>

#include "HMM.h"
#include "ObservationSeq.h"

//enum of the gesture names
//this enum will be used for the classification of the gestures
enum Gesture_Name{
	Assemble_Right_Hand,
	Assemble_Left_Hand,
	Take_Cover_Right_Hand,
	Take_Cover_Left_Hand,
	Attention_Right_Hand,
	Attenion_Left_Hand,
	//JAVID: for cover, how many times do we repeat?
	Cover_Me_Right_Hand,
	Cover_Me_Left_Hand,
	Halt_Day_Right_Hand, //pose
	Halt_Day_Left_Hand, //pose
	Halt_Night_Right_Hand,
	Halt_Night_Left_Hand,
	Danger_Area, //this gesture is only done with the right hand
	Fire_Right_Hand,
	Fire_Left_Hand,
	Commence_Firing_Right_Hand,
	Commence_Firing_Left_Hand,
	Cease_Firing_Right_Hand,
	Cease_Firing_Left_Hand,
	Contact_Left, //this gesture is only done with the left hand
	Contact_Right, //this gesture is only done with the right hand
	Action_Front_Right_Hand,
	Action_Front_Left_Hand,
	Freeze_Right_Hand, //pose
	Freeze_Left_Hand, //pose
	//no gesture for enemy in sight, ask Juan if gun has sensor that game uses to determine orientation
	Wedge_Formation, //pose
	VEE_Formation, //pose
	Line_Formation, //pose
	Traveling_Formation_Right_Hand,
	Traveling_Formation_Left_Hand,
	//PROBLEM WITH COLUMN FORMATION GESTURE, IT'S LITERALLY THE SAME AS THE ATTENTION GESTURE
	Rush_Right_Hand,
	Rush_Left_Hand,
	Quick_Time_Right_Hand,
	Quick_Time_Left_Hand,
	Mount_Right_Hand, //mount is VERY similar to take cover, but mount waves more
	Mount_Left_Hand,
	Map_Check_Right_Hand, //pose
	Map_Check_Left_Hand, //pose
	Head_Count_Right_Hand,
	Head_Count_Left_Hand,
	Do_Not_Land,
	Land, //pose
	Stop, //pose
	Move_Downward,
	Move_Upward,
	Move_Left,
	Move_Right,
	Move_Ahead,
	Move_Rearward,
	Hover, //pose
	Assume_Guidance, //pose
};


//this gesture class will contain all the information and operations 
//needed to compute the probability to recognize one gesture
class Gesture{

public:
	//getter methods
	Gesture_Name GetGestName();
	
	vector<ObservationSeq> & GetObsSamples();


	//ctors
	//the last argument to these constructors is the minimum number of elements a codebook cluster can have
	Gesture(Gesture_Name gName, const unsigned int n, const unsigned int m);
	Gesture(Gesture_Name gName, vector<ObservationSeq> obSamples, const unsigned int n, const unsigned int m);

	//function to take skeleton information, create an observation sequence out of it,
	//and add the observation sequence to the sample space
	void AddObsSeq(vector<SkeletonInfo> & siList);

	//function that saves the gesture
	void Save(ofstream & fout);

	//function that loads the gesture
	void Load(ifstream & fin);

	//the HMM for this specific gesture
	HMM gestHMM;

private:
	//enum for the gesture name
	Gesture_Name gestName;

	//the list of all the observation sequences this gesture has to work with
	vector<ObservationSeq> observSamples;

};

#endif
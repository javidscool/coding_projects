#include "GestureCollection.h"

#include <cfloat>
#include <fstream>

//ctor
//********************
//*** the codebook size MUST be the same as the number of output observations (M) of EVERY HMM 
//********************
GestureCollection::GestureCollection() : codeBook(5, 2, this), menu(this), kinectHandler(){
	
	//first we check to make sure that the Kinect was initialized successfully
	int result = kinectHandler.GetSuccessfulInit();
	//if we get an error
	if(result < 0){
		kinectError = true;
	}
	else{
		kinectError = false;
		cout << "kinect handler created successfully\n";
	}


	//JAVID: first try to make all the codebooks have the same number of clusters

	//initialize all the gestures this gesture collection can recognize
	//and pushes them back into the gestCollection list

	//i'm going to implement the feature that starts the HMM off with a small # of states'
	//and adds states until improvement peters out
	//therefore, the states i'm adding here should be on the LOW side
	Gesture AssembleRightHand(Assemble_Right_Hand, 5, 5);
	gestCollection.push_back(AssembleRightHand);

	Gesture AssembleLeftHand(Assemble_Left_Hand, 5, 5);
	gestCollection.push_back(AssembleLeftHand);

	Gesture TakeCoverRightHand(Take_Cover_Right_Hand, 4, 8);
	gestCollection.push_back(TakeCoverRightHand);

	Gesture TakeCoverLeftHand(Take_Cover_Left_Hand, 4, 8);
	gestCollection.push_back(TakeCoverLeftHand);

	Gesture AttentionRightHand(Attention_Right_Hand, 4, 8);
	gestCollection.push_back(AttentionRightHand);

	Gesture AttentionLeftHand(Attenion_Left_Hand, 4, 8);
	gestCollection.push_back(AttentionLeftHand);

	Gesture CoverMeRightHand(Cover_Me_Right_Hand, 4, 8);
	gestCollection.push_back(CoverMeRightHand);

	Gesture CoverMeLeftHand(Cover_Me_Left_Hand, 4, 8);
	gestCollection.push_back(CoverMeLeftHand);

	Gesture HaltDayRightHand(Halt_Day_Right_Hand, 3, 8);
	gestCollection.push_back(HaltDayRightHand);

	Gesture HaltDayLeftHand(Halt_Day_Left_Hand, 3, 8);
	gestCollection.push_back(HaltDayLeftHand);

	Gesture HaltNightRightHand(Halt_Night_Right_Hand, 4, 8);
	gestCollection.push_back(HaltNightRightHand);

	Gesture HaltNightLeftHand(Halt_Night_Left_Hand, 4, 8);
	gestCollection.push_back(HaltNightLeftHand);

	Gesture DangerArea(Danger_Area, 4, 8);
	gestCollection.push_back(DangerArea);

	Gesture FireRightHand(Fire_Right_Hand, 4, 8);
	gestCollection.push_back(FireRightHand);

	Gesture FireLeftHand(Fire_Left_Hand, 4, 8);
	gestCollection.push_back(FireLeftHand);

	Gesture CommenceFiringRightHand(Commence_Firing_Right_Hand, 4, 8);
	gestCollection.push_back(CommenceFiringRightHand);

	Gesture CommenceFiringLeftHand(Commence_Firing_Left_Hand, 4, 8);
	gestCollection.push_back(CommenceFiringLeftHand);

	Gesture CeaseFiringRightHand(Cease_Firing_Right_Hand, 4, 8);
	gestCollection.push_back(CeaseFiringRightHand);

	Gesture CeaseFiringLeftHand(Cease_Firing_Left_Hand, 4, 8);
	gestCollection.push_back(CeaseFiringLeftHand);

	Gesture ContactLeft(Contact_Left, 4, 8);
	gestCollection.push_back(ContactLeft);

	Gesture ContactRight(Contact_Right, 4, 8);
	gestCollection.push_back(ContactRight);

	Gesture ActionFrontRightHand(Action_Front_Right_Hand, 4, 8);
	gestCollection.push_back(ActionFrontRightHand);

	Gesture ActionFrontLeftHand(Action_Front_Left_Hand, 4, 8);
	gestCollection.push_back(ActionFrontLeftHand);

	Gesture FreezeRightHand(Freeze_Right_Hand, 3, 8);
	gestCollection.push_back(FreezeRightHand);

	Gesture FreezeLeftHand(Freeze_Left_Hand, 3, 8);
	gestCollection.push_back(FreezeLeftHand);

	Gesture WedgeFormation(Wedge_Formation, 3, 8);
	gestCollection.push_back(WedgeFormation);

	Gesture VEEFormation(VEE_Formation, 3, 8);
	gestCollection.push_back(VEEFormation);

	Gesture LineFormation(Line_Formation, 3, 8);
	gestCollection.push_back(LineFormation);

	Gesture TravelingFormationRightHand(Traveling_Formation_Right_Hand, 4, 8);
	gestCollection.push_back(TravelingFormationRightHand);

	Gesture TravelingFormationLeftHand(Traveling_Formation_Left_Hand, 4, 8);
	gestCollection.push_back(TravelingFormationLeftHand); 

	Gesture RushRightHand(Rush_Right_Hand, 4, 8);
	gestCollection.push_back(RushRightHand);

	Gesture RushLeftHand(Rush_Left_Hand, 4, 8);
	gestCollection.push_back(RushLeftHand);

	Gesture QuickTimeRightHand(Quick_Time_Right_Hand, 4, 8);
	gestCollection.push_back(QuickTimeRightHand);

	Gesture QuickTimeLeftHand(Quick_Time_Left_Hand, 4, 8);
	gestCollection.push_back(QuickTimeLeftHand);

	Gesture MountRightHand(Mount_Right_Hand, 4, 8);
	gestCollection.push_back(MountRightHand);

	Gesture MountLeftHand(Mount_Left_Hand, 4, 8);
	gestCollection.push_back(MountLeftHand);

	Gesture MapCheckRightHand(Map_Check_Right_Hand, 3, 8);
	gestCollection.push_back(MapCheckRightHand);

	Gesture MapCheckLeftHand(Map_Check_Left_Hand, 3, 8);
	gestCollection.push_back(MapCheckLeftHand);

	Gesture HeadCountRightHand(Head_Count_Right_Hand, 4, 8);
	gestCollection.push_back(HeadCountRightHand);

	Gesture HeadCountLeftHand(Head_Count_Left_Hand, 4, 8);
	gestCollection.push_back(HeadCountLeftHand);

	Gesture DoNotLand(Do_Not_Land, 4, 8);
	gestCollection.push_back(DoNotLand);

	Gesture LandGest(Land, 3, 8);
	gestCollection.push_back(LandGest);

	Gesture StopGesture(Stop, 3, 8);
	gestCollection.push_back(StopGesture);

	Gesture MoveDownward(Move_Downward, 4, 8);
	gestCollection.push_back(MoveDownward);

	Gesture MoveUpward(Move_Upward, 4, 8);
	gestCollection.push_back(MoveUpward);

	Gesture MoveLeft(Move_Left, 4, 8);
	gestCollection.push_back(MoveLeft);

	Gesture MoveRight(Move_Right, 4, 8);
	gestCollection.push_back(MoveRight);

	Gesture MoveAhead(Move_Ahead, 4, 8);
	gestCollection.push_back(MoveAhead);

	Gesture MoveRearward(Move_Rearward, 4, 8);
	gestCollection.push_back(MoveRearward);

	Gesture HoverGesture(Hover, 3, 8);
	gestCollection.push_back(HoverGesture);

	Gesture AssumeGuidance(Assume_Guidance, 3, 8);
	gestCollection.push_back(AssumeGuidance);
}


//function to check if the kinect was initialized properly
bool GestureCollection::GetKinectError(){
	return kinectError;
}


//this method will take a gesture and classify it
//DON'T CHANGE THE O THO
GestureResult GestureCollection::ClassifyGesture(ObservationSeq & O){
	time_t tStamp;

	Gesture_Name gestResultName;

	double maxLogP = -(DBL_MAX);
	double logP;

	//first create the cluster list for this observation sequence
	codeBook.CreateClusterListForClassification(O);

	//use each gesture's HMM forward method to get the highest probability and return it
	//for(int i = 0; i < gestCollection.size(); i++){

	//We do this because we're only putting in info for the first 4 gestures
	//TODO:  Delete this after we test everything
	for(int i = 0; i < 2; i++){
		//cout << "A is: " << gestCollection[i].gestHMM.getA();
		//cout << "B is: " << gestCollection[i].gestHMM.getB();
		//cout << "Pi is: " << gestCollection[i].gestHMM.getPi();

		logP = gestCollection[i].gestHMM.GiveProbabilityOfObsSeq(O);

		//if this gesture gives us the highest probability...
		if(logP > maxLogP){
			maxLogP = logP;

			//...record it's info.
			time(&tStamp);

			gestResultName = gestCollection[i].GetGestName();
		}
	}

	//create our gesture result and return it
	GestureResult gResult(gestResultName, tStamp, maxLogP);

	return gResult;
}

//function to build the codebook
void GestureCollection::BuildTheCodeBook(){
	codeBook.BuildCodeBook();
}


//function that saves everything important so we can do classification later
void GestureCollection::Save(){
	//we're saving everything we need to do classification later
	
	//first, we save the codebook
	codeBook.Save();

	//now save all the gestures
	ofstream fout("hmm.dat");

	for(int i = 0; i < gestCollection.size(); i++){
		gestCollection[i].Save(fout);
		
		//putting an endl after every save EXCEPT at the last write
		if(i < gestCollection.size() - 1)
			fout << endl;
	}
	
	//close the connection
	fout.close();
}

//function that loads everything important so we can do classification
void GestureCollection::Load(){
	//we're loading everything back up to do classification

	//first, let's load the codebook
	codeBook.Load();

	//now load all the gestures
	ifstream fin("hmm.dat");

	for(int i = 0; i < gestCollection.size(); i++){
		gestCollection[i].Load(fin);
	}
	
	//close the connection
	fin.close();
}
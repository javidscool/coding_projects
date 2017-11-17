#include "Menu.h"

//including GestureCollection.h here cause of the forward declaration in the .h file
#include "GestureCollection.h"

Menu::Menu(GestureCollection *gColl){
	gestColl = gColl;
}


//starting up the menu
void Menu::Start(){
	cout << "Do you want to train or classify?\n";

	cout << "1) Train\n2) Classify\n";

	int selection;

	cin >> selection;

	if(selection == 1)
		TrainingChoice();
	else if(selection == 2)
		ClassificationChoice();
	else{
		cout << "Invalid Selection.  Quitting\n";

		Quit();
	}
}


//shuts down the Kinect and quits
void Menu::Quit(){
	gestColl->kinectHandler.KinectUnInit();
}

//if you want to train HMMs then this provides all the choices that you can pick, from entering information to building the codebook to training your HMMs
void Menu::TrainingChoice(){
	//keep going till we quit
	while(!gestColl->kinectHandler.stopStuff){
		cout << "\nTRAINING MODE\nWhat do you wanna do:\n\n";

		cout << "\n1) Train a gesture\n";
		cout << "\n2) Build the codebook\n";
		cout << "\n3) Train the HMMs\n";
		cout << "\n4) Classify Gesture\n";
		cout << "\n5) Save\n";
		cout << "\n6) Quit\n";

		int selection;

		cin >> selection;

		switch(selection){

		case 1:
			GestureTrainChoice();

			break;

		case 2:
			gestColl->BuildTheCodeBook();

			break;

		case 3:
			//now that we've built the codebook and have our synthetic samples, initialize all the HMMs that the gestures contain
			//for(int i = 0; i < gestColl->gestCollection.size(); i++){
			for(int i = 0; i < 2; i++){
				gestColl->gestCollection[i].gestHMM.InitializeHMM(&(gestColl->gestCollection[i]));
			}

			//now that we've initialized our HMMs, we need to train them
			//for(int i = 0; i < gestColl->gestCollection.size(); i++){
			for(int i = 0; i < 2; i++){
				gestColl->gestCollection[i].gestHMM.TrainHMM();

				cout << "A is: " << gestColl->gestCollection[i].gestHMM.getA() << endl << endl;
				cout << "B is: " << gestColl->gestCollection[i].gestHMM.getB() << endl << endl;
				cout << "Pi is: " << gestColl->gestCollection[i].gestHMM.getPi() << endl << endl;
			}

			break;

		case 4:
			GetReady(3);

			break;

		case 5:
			//saves all the HMM information so we can classify with it next time the program starts
			gestColl->Save();

			break;

		case 6:
			Quit();

			break;

		default:
			cout << "\nInvalid selection, try again.\n";
		}
	}
}


//prints out choices and trains one of them (that you pick)
void Menu::GestureTrainChoice(){
	cout << "\nPlease choose a gesture to input a training gesture to:\n\n";

	cout << "1) Assemble Right Hand\n";
	cout << "2) Assemble Left Hand\n";
	cout << "3) Take Cover Right Hand\n";
	cout << "4) Take Cover Left Hand\n";
	cout << "5) Attention Right Hand\n";
	cout << "6) Attention Left Hand\n";
	cout << "7) Cover Me Right Hand\n";
	cout << "8) Cover Me Left Hand\n";
	cout << "9) Halt Day Right Hand\n";
	cout << "10) Halt Day Left Hand\n";
	cout << "11) Halt Night Right Hand\n";
	cout << "12) Halt Night Left Hand\n";
	cout << "13) Danger Area\n";
	cout << "14) Fire Right Hand\n";
	cout << "15) Fire Left Hand\n";
	cout << "16) Commence Firing Right Hand\n";
	cout << "17) Commence Firing Left Hand\n";
	cout << "18) Cease Firing Right Hand\n";
	cout << "19) Cease Firing Left Hand\n";
	cout << "20) Contact Left\n";
	cout << "21) Contact Right\n";
	cout << "22) Attention Front Right Hand\n";
	cout << "23) Attention Front Left Hand\n";
	cout << "24) Freeze Right Hand\n";
	cout << "25) Freeze Left Hand\n";
	cout << "26) Wedge Formation\n";
	cout << "27) VEE Formation\n";
	cout << "28) Line Formation\n";
	cout << "29) Traveling Formation Right Hand\n";
	cout << "30) Traveling Formation Left Hand\n";
	cout << "31) Rush Right Hand\n";
	cout << "32) Rush Left Hand\n";
	cout << "33) Quick Time Right Hand\n";
	cout << "34) Quick Time Left Hand\n";
	cout << "35) Mount Right Hand\n";
	cout << "36) Mount Left Hand\n";
	cout << "37) Map Check Right Hand\n";
	cout << "38) Map Check Left Hand\n";
	cout << "39) Head Count Right Hand\n";
	cout << "40) Head Count Left Hand\n";
	cout << "41) Do Not Land\n";
	cout << "42) Land\n";
	cout << "43) Stop\n";
	cout << "44) Move Downward\n";
	cout << "45) Move Upward\n";
	cout << "46) Move Left\n";
	cout << "47) Move Right\n";
	cout << "48) Move Ahead\n";
	cout << "49) Move Rearward\n";
	cout << "50) Hover\n";
	cout << "51) Assume Guidance\n\n";

	int selection;

	cin >> selection;

	//once we've got our selection, get ready to stand in front of the kinect
	GetReady(3, selection);
}

//takes in the number of seconds to wait while you stand in front of the Kinect
//this GetReady takes in the selection of the gesture you want to train and trains it
void Menu::GetReady(int secs, int selection){
	time_t start;
	time_t end;

	time(&start);
	time(&end);

	double gap = difftime(end, start);

	//wait three seconds so we can stand in front of the kinect
	while(gap <= secs){
		time(&end);

		gap = difftime(end, start);

		if(gap >= 2){
			cout << "\n\n2\n\n";
			continue;
		}

		if(gap >= 1){
			cout << "\n\n1\n\n";
			continue;
		}
	}

	//now that the amount of time we specified has passed, let's do what we came here to do
	AddObsSequence(3, selection);

}

//takes in selection for gesture you want to train and adds an observation sequence to the proper gesture
//takes number of secs of kinect data, transforms the kinect data to cm and makes an observation sequence out of the feature vectors we get from the Kinect
void Menu::AddObsSequence(int secs, int selection){
	//making a vector of skeleton information so we can add an observation sequence to our gesture collection for the gesture we chose
	vector<SkeletonInfo> skeletonObservation;

	time_t start;
	time_t end;

	time(&start);
	time(&end);

	double gap = difftime(end, start);

	cout << "START GESTURING!!!!!\n";

	
	//because the Sleep function is fucked up and doesn't give me back control with millisecond precision,
	//i'm qureying the time a little bit, and looping around I'm not going to get precise 1/60 of a second readings
	//this is fine, just adjust this constant to get the number of training samples you need...it's still tempering the input
	const int frameTime = 15;

	//int representing minimum timing threshold to call sleep cause sleep is innacurate with low milli numbers
	const int sleepThresh = 5;

	DWORD start_frame;
	DWORD end_frame;

	//start at frameTime's value so we start shit properly
	DWORD timeToNextFrame = 15;

	//getting timestamps in milliseconds for end_frame to start things off properly
	end_frame = GetTickCount();

	//get three seconds of kinect data
	while(gap <= secs){
		//cout << "I'M GETTING HERE\n";

		start_frame = GetTickCount();
		
		while(start_frame < (end_frame + timeToNextFrame)) {
			YieldProcessor();
			
			start_frame = GetTickCount();
		}

		//mark the start and end of how long it takes to get a kinect skeleton frame
		start_frame = GetTickCount();

		//getting the latest skeleton from the kinect
		skeletonObservation.push_back(gestColl->kinectHandler.SendKinectSkeleton());

		end_frame = GetTickCount();


		//seeing how many milliseconds we have to wait for the next skeleton catch
		timeToNextFrame = frameTime - (end_frame - start_frame);

		//if we have a time big enough to sleep for, sleep
		if(timeToNextFrame > sleepThresh){
			Sleep(timeToNextFrame);
		}


		time(&end);

		gap = difftime(end, start);
	}

	//cout << "skeleton count: " << skeletonObservation.size() << endl;

	//after we get three seconds of kinect data, create an observation sequence out of it
	gestColl->gestCollection[selection - 1].AddObsSeq(skeletonObservation);
}

//getting kinect skeleton info from the kinect handler
SkeletonInfo Menu::GetKinectSkeleton(){

	//get the current skeleton info from the kinect
	return gestColl->kinectHandler.SendKinectSkeleton();
}

//if you want to just classify shit and you have your HMM then this runs
void Menu::ClassificationChoice(){
	//first we need to load the HMM information so we can classify with it
	gestColl->Load();

	//keep going till we quit
	while(!gestColl->kinectHandler.stopStuff){
		cout << "\nCLASSIFICATION MODE\nWhat do you wanna do:\n\n";

		cout << "\n1) Classify a gesture\n";
		cout << "\n2) Quit\n";

		int selection;

		cin >> selection;

		switch(selection){

		case 1:
			GetReady(3);

			break;

		case 2:
			Quit();

			break;

		default:
			cout << "\nInvalid selection, try again.\n";
		}
	}
}

//takes in the number of seconds to wait while you stand in front of the Kinect
//this get ready is used for classification of a gesture you do
void Menu::GetReady(int secs){
	time_t start;
	time_t end;

	time(&start);
	time(&end);

	double gap = difftime(end, start);

	//wait three seconds so we can stand in front of the kinect
	while(gap <= secs){
		time(&end);

		gap = difftime(end, start);

		if(gap >= 2){
			cout << "\n\n2\n\n";
			continue;
		}

		if(gap >= 1){
			cout << "\n\n1\n\n";
			continue;
		}
	}

	//now that the amount of time we specified has passed, let's do what we came here to do
	GetObsSequence(3);

}


//gets an observation sequence from the kinect and sends it off to be classified
//takes number of secs of kinect data, transforms the kinect data to cm and makes an observation sequence out of the feature vectors we get from the Kinect
void Menu::GetObsSequence(int secs){
	//making a vector of skeleton information so we can get an observation sequence to classify
	vector<SkeletonInfo> skeletonObservation;

	time_t start;
	time_t end;

	time(&start);
	time(&end);

	double gap = difftime(end, start);

	cout << "START GESTURING!!!!!\n";

	
	//because the Sleep function is fucked up and doesn't give me back control with millisecond precision,
	//i'm qureying the time a little bit, and looping around I'm not going to get precise 1/60 of a second readings
	//this is fine, just adjust this constant to get the number of training samples you need...it's still tempering the input
	const int frameTime = 15;

	//int representing minimum timing threshold to call sleep cause sleep is innacurate with low milli numbers
	const int sleepThresh = 5;

	DWORD start_frame;
	DWORD end_frame;

	//start at frameTime's value so we start shit properly
	DWORD timeToNextFrame = 15;

	//getting timestamps in milliseconds for end_frame to start things off properly
	end_frame = GetTickCount();

	//get three seconds of kinect data
	while(gap <= secs){
		
		start_frame = GetTickCount();
		
		while(start_frame < (end_frame + timeToNextFrame)) {
			YieldProcessor();
			
			start_frame = GetTickCount();
		}

		//mark the start and end of how long it takes to get a kinect skeleton frame
		start_frame = GetTickCount();

		//getting the latest skeleton from the kinect
		skeletonObservation.push_back(gestColl->kinectHandler.SendKinectSkeleton());

		end_frame = GetTickCount();

		//seeing how many milliseconds we have to wait for the next skeleton catch
		timeToNextFrame = frameTime - (end_frame - start_frame);

		//if we have a time big enough to sleep for, sleep
		if(timeToNextFrame > sleepThresh){
			Sleep(timeToNextFrame);
		}

		time(&end);

		gap = difftime(end, start);
	}

	//after we get three seconds of kinect data, create an observation sequence out of it
	ObservationSeq gestToClassify(skeletonObservation);

	ClassifyMyGest(gestToClassify);
}


//takes in an observation sequence that we enter and classifies it
void Menu::ClassifyMyGest(ObservationSeq & O){
	//classify this gesture
	GestureResult gResult = gestColl->ClassifyGesture(O);

	switch(gResult.resultName){
	case Assemble_Right_Hand:
		cout << "\nWe did assemble right hand.\n";
		break;

	case Assemble_Left_Hand:
		cout << "\nWe did assemble left hand.\n";
		break;

	case Take_Cover_Right_Hand:
		cout << "\nWe did take cover right hand.\n";
		break;

	case Take_Cover_Left_Hand:
		cout << "\nWe did take cover left hand.\n";
		break;

	case Attention_Right_Hand:
		cout << "\nWe did attention right hand.\n";
		break;

	case Attenion_Left_Hand:
		cout << "\nWe did attention left hand.\n";
		break;

	case Cover_Me_Right_Hand:
		cout << "\nWe did cover me right hand.\n";
		break;

	case Cover_Me_Left_Hand:
		cout << "\nWe did cover me left hand.\n";
		break;

	case Halt_Day_Right_Hand:
		cout << "\nWe did halt day right hand.\n";
		break;

	case Halt_Day_Left_Hand:
		cout << "\nWe did halt day left hand.\n";
		break;

	case Halt_Night_Right_Hand:
		cout << "\nWe did halt night right hand.\n";
		break;

	case Halt_Night_Left_Hand:
		cout << "\nWe did halt night left hand.\n";
		break;

	case Danger_Area:
		cout << "\nWe did danger area.\n";
		break;

	case Fire_Right_Hand:
		cout << "\nWe did fire right hand.\n";
		break;

	case Fire_Left_Hand:
		cout << "\nWe did fire left hand.\n";
		break;

	case Commence_Firing_Right_Hand:
		cout << "\nWe did commence firing right hand.\n";
		break;

	case Commence_Firing_Left_Hand:
		cout << "\nWe did commence firing left hand.\n";
		break;

	case Cease_Firing_Right_Hand:
		cout << "\nWe did cease firing right hand.\n";
		break;

	case Cease_Firing_Left_Hand:
		cout << "\nWe did cease firing left hand.\n";
		break;

	case Contact_Left:
		cout << "\nWe did contact left.\n";
		break;

	case Contact_Right:
		cout << "\nWe did contact right.\n";
		break;

	case Action_Front_Right_Hand:
		cout << "\nWe did action front right hand.\n";
		break;

	case Action_Front_Left_Hand:
		cout << "\nWe did action front left hand.\n";
		break;

	case Freeze_Right_Hand:
		cout << "\nWe did freeze right hand.\n";
		break;

	case Freeze_Left_Hand:
		cout << "\nWe did freeze left hand.\n";
		break;

	case Wedge_Formation:
		cout << "\nWe did wedge formation.\n";
		break;

	case VEE_Formation:
		cout << "\nWe did VEE formation.\n";
		break;

	case Line_Formation:
		cout << "\nWe did line formation.\n";
		break;

	case Traveling_Formation_Right_Hand:
		cout << "\nWe did traveling formation right hand.\n";
		break;

	case Traveling_Formation_Left_Hand:
		cout << "\nWe did traveling formation left hand.\n";
		break;

	case Rush_Right_Hand:
		cout << "\nWe did rush right hand.\n";
		break;

	case Rush_Left_Hand:
		cout << "\nWe did rush left hand.\n";
		break;

	case Quick_Time_Right_Hand:
		cout << "\nWe did quick time right hand.\n";
		break;

	case Quick_Time_Left_Hand:
		cout << "\nWe did quick time left hand.\n";
		break;

	case Mount_Right_Hand:
		cout << "\nWe did mount right hand.\n";
		break;

	case Mount_Left_Hand:
		cout << "\nWe did mount left hand.\n";
		break;

	case Map_Check_Right_Hand:
		cout << "\nWe did map check right hand.\n";
		break;

	case Map_Check_Left_Hand:
		cout << "\nWe did map check left hand.\n";
		break;

	case Head_Count_Right_Hand:
		cout << "\nWe did head count right hand.\n";
		break;

	case Head_Count_Left_Hand:
		cout << "\nWe did head count left hand.\n";
		break;

	case Do_Not_Land:
		cout << "\nWe did do not land.\n";
		break;

	case Land:
		cout << "\nWe did land.\n";
		break;

	case Stop:
		cout << "\nWe did stop.\n";
		break;

	case Move_Downward:
		cout << "\nWe did move downward.\n";
		break;

	case Move_Upward:
		cout << "\nWe did move upward.\n";
		break;

	case Move_Left:
		cout << "\nWe did move left.\n";
		break;

	case Move_Right:
		cout << "\nWe did move right.\n";
		break;

	case Move_Ahead:
		cout << "\nWe did move ahead.\n";
		break;

	case Move_Rearward:
		cout << "\nWe did move rearward.\n";
		break;

	case Hover:
		cout << "\nWe did hover.\n";
		break;

	case Assume_Guidance:
		cout << "\nWe did assume guidance.\n";
		break;

	default:
		cout << "\nCouldn't recognize your gesture.\n";

	}
}
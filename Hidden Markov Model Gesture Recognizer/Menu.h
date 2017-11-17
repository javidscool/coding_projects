#ifndef MENU_H
#define MENU_H

#include <ctime>
#include <iostream>
#include <vector>

#include "ObservationSeq.h"

//forward declaration
class GestureCollection;

//struct GestureResult;

using namespace std;

class Menu{

public:
	Menu(GestureCollection *gColl); //DONE

	//shuts down the Kinect and quits
	void Quit(); //DONE

	//starting up the menu
	void Start(); //DONE


	//**********************Training Menu Functions**********************************************

	//if you want to train HMMs then this provides all the choices that you can pick, from entering information to building the codebook to training your HMMs
	void TrainingChoice(); //DONE

	//prints out choices and trains one of them (that you pick)
	void GestureTrainChoice(); //DONE



	//**********************Classification Menu Functions****************************************

	//if you want to just classify shit and you have your HMM then this runs
	void ClassificationChoice();


private:
	//takes in the number of seconds to wait while you stand in front of the Kinect
	//this GetReady takes in the selection of the gesture you want to train and trains it
	void GetReady(int secs, int selection);  //DONE

	//takes in the number of seconds to wait while you stand in front of the Kinect
	//this get ready is used for classification of a gesture you do
	void GetReady(int secs); //DONE

	//takes in selection for gesture you want to train and adds an observation sequence to the proper gesture
	//takes number of secs of kinect data, transforms the kinect data to cm and makes an observation sequence out of the feature vectors we get from the Kinect
	void AddObsSequence(int secs, int selection); //DONE

	//gets an observation sequence from the kinect and sends it off to be classified
	//takes number of secs of kinect data, transforms the kinect data to cm and makes an observation sequence out of the feature vectors we get from the Kinect
	void GetObsSequence(int secs); //DONE

	//takes in an observation sequence that we enter and classifies it
	void ClassifyMyGest(ObservationSeq & O);



	//getting kinect skeleton info from the kinect handler
	SkeletonInfo GetKinectSkeleton();

	GestureCollection *gestColl;

};

#endif
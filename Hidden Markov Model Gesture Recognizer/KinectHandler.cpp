#include "KinectHandler.h"

using namespace std;

KinectHandler::KinectHandler(void)
{
	cout << "Starting kinect handler\n";

	cout << "I've created a kinect handler\n";

	//reset this kinect handler before initializing it
	KinectZero();

	//initialize this kinect handler
	long result = KinectInit();
	
	if(result < 0)
		successfulInit = false;
	else{
		successfulInit = true;
		stopStuff = false;
	}
}


KinectHandler::~KinectHandler(void)
{
	cout << "I'm destructing a kinect handler\n";

	KinectZero();
}

void KinectHandler::KinectZero()
{
	nuiProcess = NULL;
	processStop = NULL;
	nextSkeletonEvent = NULL;
	kinMutex = NULL;
	successfulInit = false;
	stopStuff = true;
}

long KinectHandler::KinectInit()
{
	long result;

	//create an event for skeleton tracking 
	nextSkeletonEvent = CreateEvent(NULL, true, false, NULL);


	//initializing kinect shit
	result = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
	
	if(result < 0){
		cout << "Initialization failed\n";
		return result;
	}

	else{
		cout << "Initialization succeeded!!!\n";
	}


	//enable skeleton tracking
	result = NuiSkeletonTrackingEnable(nextSkeletonEvent, 0);
	
	if(result < 0){
		cout << "Skeleton tracking failed\n";
		return result;
	}

	else{
		cout << "Skeleton tracking succeeded!!!\n";
	}

	//creating the mutex for the shared current kinect skeleton
	kinMutex = CreateMutex(NULL, FALSE, NULL);
	if(kinMutex == NULL){
		cout << "Mutex creation failed.\n";
		return -1;
	}


	//creating the stop event and starting the process
	processStop = CreateEvent(NULL, false, false, NULL);
	nuiProcess = CreateThread(NULL, 0, NUI_ProcessThread, this, 0, NULL);

	numFramesSaved = 5;

	return result;
}

void KinectHandler::KinectUnInit()
{
	cout << "I'M IN UNINIT!!!\n";

	//first stop the nui processing thread
	if(processStop != NULL)
	{
		//set the event to stop
		SetEvent(processStop);

		//wait for thread to stop
        if(nuiProcess != NULL)
        {
            WaitForSingleObject(nuiProcess, INFINITE);
            CloseHandle(nuiProcess);
        }

        CloseHandle(processStop);
	}

	//shutting down the kinect
	NuiShutdown();

	//closing the skeleton handle
	if(nextSkeletonEvent && (nextSkeletonEvent != INVALID_HANDLE_VALUE) )
    {
        CloseHandle(nextSkeletonEvent);
        nextSkeletonEvent = NULL;
    }

	//closing the mutex handle
	CloseHandle(kinMutex);

	cout << "I got to the end of un-init.\n";
}

int KinectHandler::GetSuccessfulInit()
{
	if(!successfulInit)
		return -1;

	else
		return 0;
}

DWORD WINAPI KinectHandler::NUI_ProcessThread(LPVOID pParam)
{
	cout << "I'm getting in here to NUI PROCESS THREAD!!!\n";

	//first cast the void pointer to the object that we passed in
	KinectHandler *kinectPt = (KinectHandler *)pParam;

	//creating an array of all the events that we have
	HANDLE theEvents[2];

	//the index of the event that was triggered
	int eventIndx;
	
	DWORD mutexResult;

	//configure the events that we want to listen for
	theEvents[0] = kinectPt->processStop;
	theEvents[1] = kinectPt->nextSkeletonEvent;

	//infinite loop that takes the events
	while(true){

		//first we wait until an event gets triggered or 100ms elapses
		eventIndx = WaitForMultipleObjects( sizeof(theEvents)/sizeof(theEvents[0]), theEvents, false, 100);

		//cout << "eventIndx: " << eventIndx << endl;

		//if the event we got was the stop event, break out of the loop
		if(eventIndx == 0){
			cout << "I'm BREAKING OUT!!!\n";
			kinectPt->stopStuff = true;
			break;
		}

		else if(eventIndx == 1)
		{
			//request ownership of mutex to set current kinect skeleton
			mutexResult = WaitForSingleObject(kinectPt->kinMutex, INFINITE);
			switch(mutexResult){
			//the thread got ownership of the mutex
			case WAIT_OBJECT_0:
				//set current kinect skeleton and release mutex
				kinectPt->currentKinectSkeleton =  kinectPt->GotSkeletonAlert();
				if( !ReleaseMutex(kinectPt->kinMutex) ){
					cout << "ERROR RELEASING MUTEX!!!\n";
				}
				break;
			case WAIT_ABANDONED:
				cout << "ABANDONED MUTEX! TROUBLE!!!\n";
				break;
			default:
				cout << "UNKNOWN ERROR!!!\n";
			}
		}
	}

	return 0;

}

//this method gets called when the kinect is ready to give us a new frame of skeleton
SkeletonInfo KinectHandler::GotSkeletonAlert()
{
	//cout << "I got here in the skeleton shit!!!\n";
	
	NUI_SKELETON_FRAME skeletonFrame;

	SkeletonInfo mySkeletonFrame;

	//first parameter indicates how many milliseconds to wait for a frame
	//second parameter is a pointer to a location to put the skeleton frame
	long result = NuiSkeletonGetNextFrame(0, &skeletonFrame);

	bool foundSkeleton = false;

	if(result >= 0){
		for(int i = 0; i < NUI_SKELETON_COUNT; i++){
			//if the skeleton is actively tracked
			if(skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
				foundSkeleton = true;
				//cout << "I FOUND A FUCKING SKELETON!!!\n";
			}
		}
	}

	//if no skeleton found
	if(!foundSkeleton)
	{
		//cout << "I'M NOT FINDING A SKELETON!!!\n";
		return mySkeletonFrame;
	}

	//let Microsoft smooth my skeleton out
	NuiTransformSmooth(&skeletonFrame, NULL);


	for(int i = 0; i < NUI_SKELETON_COUNT; i++){

		//show skeleton only if it is tracked, and the center-shoulder joint is at least inferred
        if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED &&
            skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_SHOULDER_CENTER] != NUI_SKELETON_POSITION_NOT_TRACKED)
        {
			//cout << "i of frame: " << i << endl << endl;
			//cout << "Position of my left hand x: " << skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x << endl;
			//cout << "Position of my left hand y: " << skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y << endl;
			//cout << "Position of my left hand z: " << skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z << endl;

			//i'm just pumping out skeleton information, always
			//if I wanna get it I just use SendKinectSkeleton from the menu class
			mySkeletonFrame = GiveKinectSkeleton(skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER],
											     skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT],
												 skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT],
												 skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT],
												 skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT],
												 skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT],
												 skeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]);
			
			return mySkeletonFrame;
		}
    }

	//if we get here then the kinect could NOT track our skeleton, so just return false cause we don't wanna quit either
	return mySkeletonFrame;
}

//giving kinect skeleton information, always
SkeletonInfo KinectHandler::GiveKinectSkeleton(Vector4 shoulder_center, Vector4 shoulder_right, Vector4 elbow_right, Vector4 hand_right, Vector4 shoulder_left, Vector4 elbow_left, Vector4 hand_left){
	//kinect gives us info. in meters
	//I convert to cm to give more varied numbers
	SkeletonInfo kinectSkeleton(JointPoint( (shoulder_center.x * 100), (shoulder_center.y * 100), (shoulder_center.z * 100) ),
								JointPoint( (shoulder_right.x * 100), (shoulder_right.y * 100), (shoulder_right.z * 100) ),
								JointPoint( (elbow_right.x * 100), (elbow_right.y * 100), (elbow_right.z * 100) ),
								JointPoint( (hand_right.x * 100), (hand_right.y * 100), (hand_right.z * 100) ),
								JointPoint( (shoulder_left.x * 100), (shoulder_left.y * 100), (shoulder_left.z * 100) ),
								JointPoint( (elbow_left.x * 100), (elbow_left.y * 100), (elbow_left.z * 100) ),
								JointPoint( (hand_left.x * 100), (hand_left.y * 100), (hand_left.z * 100) ),
								0.0,
								0.0);

	return kinectSkeleton;
}


//send out the current kinect skeleton we have
SkeletonInfo KinectHandler::SendKinectSkeleton(){

	SkeletonInfo mySkeletonFrame;

	DWORD mutexResult;

	//request ownership of mutex to get current kinect skeleton
	mutexResult = WaitForSingleObject(kinMutex, INFINITE);
	switch(mutexResult){
	//the thread got ownership of the mutex
	case WAIT_OBJECT_0:
		//get current kinect skeleton and release mutex
		//cout << "BOIBOI\n";
		mySkeletonFrame =  currentKinectSkeleton;
		if( !ReleaseMutex(kinMutex) ){
			cout << "ERROR RELEASING MUTEX!!!\n";
		}
		break;
	case WAIT_ABANDONED:
		cout << "ABANDONED MUTEX! TROUBLE!!!\n";
		break;
	default:
		cout << "UNKNOWN ERROR!!!\n";
	}

	return mySkeletonFrame;
}


double KinectHandler::DistanceCalculation(Vector4 pos1, Vector4 pos2)
{
	return sqrt( pow( (pos1.x - pos2.x), 2) + pow( (pos1.y - pos2.y), 2) + pow( (pos1.z - pos2.z), 2) ); 
}

double KinectHandler::OneDimDistanceCalculation(double pt1, double pt2)
{
	return sqrt( pow( (pt1 - pt2), 2) );
}

#ifndef KINECT_HANDLER
#define KINECT_HANDLER

#include <iostream>
#include <cmath>

#include <Windows.h>

#include <MSR_NuiApi.h>
#include <MSR_NuiProps.h>
#include <MSR_NuiSkeleton.h>

//#include <ctime>
#include <vector>

#include "resource.h"

#include "ObservationSeq.h"

class KinectHandler
{
public:
	KinectHandler(void);
	~KinectHandler(void);

	long KinectInit();
	void KinectUnInit();
	SkeletonInfo GotSkeletonAlert();

	int GetSuccessfulInit();

	bool stopStuff;

	//send out the current kinect skeleton we have
	SkeletonInfo SendKinectSkeleton();

private:
	void KinectZero();

	static DWORD WINAPI NUI_ProcessThread(LPVOID pParam);

	//thread for the process
    HANDLE nuiProcess;
	
	//the stop event
	HANDLE processStop;

	//the skeleton event
	HANDLE nextSkeletonEvent;

	bool successfulInit;

	int numFramesSaved;

	double DistanceCalculation(Vector4 pos1, Vector4 pos2);

	double OneDimDistanceCalculation(double pt1, double pt2);


	//giving kinect skeleton information, always
	SkeletonInfo GiveKinectSkeleton(Vector4 shoulder_center, Vector4 shoulder_right, Vector4 elbow_right, Vector4 hand_right, Vector4 shoulder_left, Vector4 elbow_left, Vector4 hand_left);

	SkeletonInfo currentKinectSkeleton;

	HANDLE kinMutex;

	
};

#endif


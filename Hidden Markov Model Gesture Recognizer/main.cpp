#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "GestureCollection.h"

//#include <cstdlib>
//#include <ctime>

using namespace std;
using Eigen::MatrixXd;

int main(){

	//creating the gesture collection
	GestureCollection myGestColl;

	//check to make sure the kinect got initialized properly
	if(myGestColl.GetKinectError()){
		cout << "ERROR IN KINECT INIT\n";
		system("pause");
		return 0;
	}

	//start up the menu
	myGestColl.menu.Start();

	system("pause");
	return 0;
}
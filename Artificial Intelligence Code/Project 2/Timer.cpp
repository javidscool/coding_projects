#include "Timer.h"

#include <iostream>
using std::cout;

using namespace irr;

void Timer::update(const irr::ITimer* time){
	irr::u32 ctime = 0;
	irr::f32 TIMEELAPSED = (irr::f32)((ctime = time->getTime()) - last_update);

	timer -= TIMEELAPSED;
	if(timer >= 0)
		Timer::convert();

	if(timer == 0)
		std::cout << "Time's up.\n";
	
	last_update = ctime;
}

bool Timer::processMessage(const Message* m){
	return true;
}

void Timer::convert(){
	int min;
	int sec;
	int rest;

	//for getting intermediate results
	int inter;

	//60,000 ms equals one minute
	min = timer/60000;

	//Set this as timer private
	minsRemain = min;

	inter = timer%60000;

	sec = inter/1000;

	//Set second and first order seconds remaining
	secsOrder2Remain = sec/10;
	secsOrder1Remain = sec%10;

	rest = inter % 1000;

	//cout << "\n" << min << ": " << sec << ": " << rest << "\n";
}
	

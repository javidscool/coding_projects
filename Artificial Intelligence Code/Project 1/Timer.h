#ifndef TIMER_H_
#define TIMER_H_

#include "GameEntity.h"

#include <iostream>
using std::cout;

class Timer : public GameEntity{
private:
	int timer;
	irr::u32 minsRemain, secsOrder1Remain, secsOrder2Remain;
	irr::u32 last_update;

public:
	virtual void update(const irr::ITimer*);
	virtual bool processMessage(const Message*);
	inline irr::u32 getMins(){return minsRemain;}; //returns number of minutes
	inline irr::u32 getSecsFirst(){return secsOrder1Remain;}; //returns number of seconds (s*1x10^1)
	inline irr::u32 getSecsSecond(){return secsOrder2Remain;}; //returns number of seconds (s*1x10^0)
	inline void setLastTime(irr::u32 lastTime) {last_update = lastTime;}
	Timer(int t=0) : timer(t), last_update(0) {}
	void convert();
	int getTime(){ return timer; }
	void setTime(int t){
		cout << "I reset the time.\n";
		timer = t; 
	}
};

#endif

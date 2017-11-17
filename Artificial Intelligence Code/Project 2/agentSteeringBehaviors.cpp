#include "agent.h"
#include "cpMath.h"
#include "coverObject.h"

//generates a seek steering force
irr::core::vector3df Agent::seek(irr::core::vector3df tp){
	
	if(tp == mynodep->getPosition())
		return vector3df(0,0,0);

	//irr::f32 MAXSPEED = .3f;
	irr::core::vector3df target = tp - mynodep->getPosition();
	target.Y = 0;
	if(target.getLength() == 0)
		return vector3df(0,0,0);
	target.normalize();
	target*=ACCELRATE;
	//irr::f32 mass = 10.0f;

	irr::core::vector3df accel = (target-velocity);
	accel/=mass;
	//accel*= (tp - mynodep->getPosition()).getLength()*.05;
	return accel;
}

//generates a flee steering force
irr::core::vector3df Agent::flee(irr::core::vector3df tp){
	if(tp == mynodep->getPosition())
		return vector3df(0,0,0);
	
	//irr::f32 MAXSPEED = .3f;
	irr::core::vector3df target = -tp + mynodep->getPosition();
	target.Y = 0;
	if(target.getLength() == 0)
		return vector3df(0,0,0);
	
	target.normalize();
	target*=ACCELRATE;
	//irr::f32 mass = 10.0f;

	irr::core::vector3df accel = (target-velocity);
	accel/=mass;
	//accel*= (tp - mynodep->getPosition()).getLength()*.05;
	return accel;	
}

//updates animation/rotation, moves the agent along its velocity
void Agent::walk(irr::core::vector3df accel){
	
	if(!(velocity+(accel*TIMEELAPSED)).getLength() == 0.0f){
		velocity += accel*TIMEELAPSED;
		
		if(velocity.getLength() > MAXSPEED){
			velocity = velocity.normalize()*MAXSPEED;
		}
	}
	else{
		velocity = vector3df(0,0,0);
	}
	
	if(velocity.getLength() > .01f){
		irr::core::vector3df ppos = mynodep->getPosition();
		mynodep->setPosition(mynodep->getPosition() + (TIMEELAPSED * velocity));
		position = mynodep->getPosition();
		mynodep->setPosition( ppos + (TIMEELAPSED * velocity));

		if(!MOVING){
			MOVING= true;
			((irr::scene::IAnimatedMeshSceneNode*)mynodep)->setMD2Animation(scene::EMAT_RUN);
		}
	}
	else if(MOVING){
		MOVING = false;
		((irr::scene::IAnimatedMeshSceneNode*)mynodep)->setMD2Animation(scene::EMAT_STAND);
	}

	vector3df abc = velocity;//SEEK_POS - mynodep->getPosition();
	abc.Y = 0;
	abc = abc.normalize();
	double tAngle = radiansToDegrees(acos(fabs(abc.X)));
	
	switch( quadrant(velocity.normalize() ) ){
		case 1:	break;
		case 2:	tAngle = 180-tAngle;
				break;
		case 3:	tAngle = 180+tAngle;
				break;
		case 4:	tAngle = 360-tAngle;
				break;
		default:;
	}
	
	if(velocity.getLength()!=0){
		orientation = tAngle;
		mynodep->setRotation(irr::core::vector3df(0.0f,(irr::f32)fabs(360-orientation),0.0f));
	}
	
	position = mynodep->getPosition();
}


irr::core::vector3df Agent::wallAvoidance(){
	irr::core::vector3df wallavoidaccel;
	wallavoidaccel = vector3df(0,0,0);
	for(int i = 0; i < s1d->getNumFeelers(); i++){
		//if(s1d->feelerDistances[i] < 50){
			//wallavoidaccel+=s1d->triangle[i].getNormal()*(1/(s1d->feelerDistances[i]*s1d->feelerDistances[i]));
			//if(velocity.getLength() - s1d->feelerDistances[i] > 0.0){
		double tmp = (s1d->maxRange - s1d->feelerDistances[i]);
		tmp/=5000000;//1
		//if(tmp>0.0f){
		wallavoidaccel+=s1d->triangle[i].getNormal()*tmp;
		//std::cout<<s1d->feelerDistances[i]<<"\n";

		
	//}
	//}
	//}
	}
	
	wallavoidaccel.Y = 0;
	//wallavoidaccel = wallavoidaccel.normalize(); 

	if(wallavoidaccel.getLength() > .025f){
		wallavoidaccel = wallavoidaccel.normalize();
		wallavoidaccel*=.025f;
	}

	if(wallavoidaccel.getLength() != 0){
		//std::cout<<wallavoidaccel.getLength()<<"\n";
	}
	
	return wallavoidaccel;
}

static const double EXTRA_RADIUS = 30;

irr::core::vector3df Agent::followPath(const irr::ITimer* timer){
	
	//seek to the current seek target
	vector3df tp = currentSeekTarget;
	tp.Y = 0;
	vector3df ap = mynodep->getPosition();
	ap.Y = 0;
	tp = tp-ap;
	
	core::vector3df tv = (-mynodep->getPosition() + currentSeekTarget);
	
	tv.Y = 0;
	
	if(tv.getLength()<RADIUS){
		if(!pathList.empty()){
			//std::cout<<"popping node off list\n";
			previousSeekTarget = currentSeekTarget;//mynodep->getPosition();
			currentSeekTarget = pathList.front();
			pathList.erase(pathList.begin()); //ZOMG WTF obscure bug avoidance tip #666 : don't use list.remove(pathList.begin()) when you mean list.erase(pathList.begin())
	
			//std::cout<<"Arrival\n";
			int p = this->graph->getClosestNode(previousSeekTarget);
			int q = this->graph->getClosestNode(currentSeekTarget);
			//std::cout<<"Going from"<<p<<"to"<<q<<std::endl;
			//if(graph->adjacencyList[p][q]){
			//std::cout<<"ok\n";
			//}else{
			//std::cout<<"WTF BAD EDGE POPPED\n";
			//}

			this->pathStartTime = timer->getTime();
			this->expectedArrivalTime = pathStartTime+(currentSeekTarget - this->getPosition()).getLength() /  MAXSPEED;
		}
		else{
			velocity = core::vector3df(0,0,0);
			currentSeekTarget = mynodep->getPosition();
			previousSeekTarget = mynodep->getPosition();
			this->pathStartTime = timer->getTime();
			this->expectedArrivalTime = pathStartTime+(currentSeekTarget - this->getPosition()).getLength() /  MAXSPEED;
		}
	}

	//check to see if the path needs to be corrected
	if( (timer->getTime() - this->pathStartTime) > TIMEMULTIPLIER*(this->expectedArrivalTime - this->pathStartTime) ){
		this->pathStartTime = timer->getTime();
		correctPath();		
		this->expectedArrivalTime = pathStartTime+(pathList.front() - this->getPosition()).getLength() /  MAXSPEED;
	}


	irr::core::vector3df accel;
	accel = seek(currentSeekTarget) + wallAvoidance();

	/////////////////////////////////////////////////begin new obstacle avoidance code:keeping this isolated!
	if( timer->getTime() - this->LAST_OBSTACLE_CORRECTANCE > 2000){
		//create the line from the agent extending outward through its velocity
		irr::core::line3d<irr::f32> line;
		line.start = mynodep->getPosition();
		line.end = line.start + (this->getVelocity().normalize() *(coverObject::getRadius() + 50));

		//get the scene node it intersects
		irr::scene::ISceneNode* tnode; 
		tnode= smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(line);
		coverObject* coverObj= NULL;

		if(tnode){
			//get the cover object it represents;
			for(int i = 0; i < (*this->coverObjectList).size(); i++){
				if(tnode == (*this->coverObjectList)[i]->getSceneNode()){
					coverObj = (*this->coverObjectList)[i];
					break;
				}
			}
			//if its a cover object
			if(coverObj){
				//and i'm close to it
				if( (this->getSceneNode()->getPosition() - coverObj->getSceneNode()->getPosition()).getLength() < coverObj->getBoundaryRadius()){
					std::cout<<"I'm too near the cover objects!\n";
					irr::core::vector3df startVector;
					irr::core::vector3df endVector;

					startVector =  mynodep->getPosition() - coverObj->getSceneNode()->getPosition();
					startVector = startVector.normalize();

					endVector = this->currentSeekTarget - coverObj->getSceneNode()->getPosition();
					endVector = endVector.normalize();

					double startAngle = vectorAngle(startVector);
					double endAngle = vectorAngle(endVector);
					const double nodeCount = 8;
					//get the angle of each of these vectors so we can generate a path correction arc
			
					std::list<irr::core::vector3df> result(nodeCount);

					const irr::core::vector3df& canPos =coverObj->getSceneNode()->getPosition();

					double increment = (startAngle - endAngle) / nodeCount;

					for(int i = 0; i < nodeCount; i++){
						double currentAngle = endAngle + increment*i;
				
						//this->pathListAvd.push_front(
						//result.push_back(

						pathList.push_front( (coverObj->getBoundaryRadius() + EXTRA_RADIUS)*vector3df( cos(currentAngle)  ,0,sin(currentAngle) ) + canPos);
						//irr::scene::ISceneNode* a = this->smgr->addSphereSceneNode(5);
						//a->setPosition(result.back());
					}

					previousSeekTarget = mynodep->getPosition();
					currentSeekTarget = pathList.front();
					this->pathStartTime = timer->getTime();
					this->expectedArrivalTime = pathStartTime+(currentSeekTarget - this->getPosition()).getLength() /  MAXSPEED;
					this->LAST_OBSTACLE_CORRECTANCE = timer->getTime();
					this->setVelocity(vector3df(0,0,0));
				}
			}
		}
	}
	/////////////////////////////////////////end new obstacle avoidance code
	
	return accel;
}




irr::core::vector3df Agent::pursue(physicsObject* tgt){
	const double constant = 1.0f;
	double lookAheadTime = (this->getPosition() - tgt->getPosition()).getLength();
	lookAheadTime /= (.45);//(this->getVelocity().getLength() + tgt->getVelocity().getLength());
	lookAheadTime *= constant;

	return seek(tgt->getPosition() + lookAheadTime* tgt->getVelocity());
}


irr::core::vector3df Agent::avoid(physicsObject* tgt){
	
	const double constant = 1.0f;
	double lookAheadTime = (this->getPosition() - tgt->getPosition()).getLength();
	lookAheadTime /= (this->getVelocity().getLength() + tgt->getVelocity().getLength());
	lookAheadTime *= constant;

	return flee(tgt->getPosition() + lookAheadTime* tgt->getVelocity());
}

irr::core::vector3df Agent::hide(){
	if(!myCoverObject)
		return vector3df(0,0,0);
	
	return this->seek(myCoverObject->getCoverPosition(this->getIt()));
}





#include "Agent.h"
#include "cpMath.h"


void Agent::updateWallSensor(){
	core::line3d<f32> line;
	core::vector3df intersection;
	core::triangle3df triangle;
	core::vector3df orientVector;
 
	orientVector = core::vector3df((float)cos(degreesToRadians(orientation)),0.0f,(float)sin(degreesToRadians(orientation)));
	line.start = mynodep->getPosition();
	line.end = line.start + orientVector * s1d->maxRange;
 
	//billboard->setVisible(true);
		 
	//std::vector<Agent*> a;
	//a = *agentList;
	//(*agentList)[0] = NULL;
	//a[0] = NULL;
 
	double baseAngle = -.5f*s1d->getAngle();
 
	baseAngle +=orientation;
 
	double increment = s1d->getAngle() / (double)s1d->getNumFeelers();
	
	for(int i = 0; i < s1d->getNumFeelers(); i++){
		double angle = i * increment + baseAngle;
		core::vector3df feelerVector = core::vector3df((float)cos(degreesToRadians(angle)), 0.0f, (float)sin(degreesToRadians(angle)));

		float t1 = 0.0f;
			 
		line.start = mynodep->getPosition();
		line.end = line.start + feelerVector * s1d->maxRange;
		//printf("s1dmr%d\n", s1d->maxRange();
 
		if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
			//if(feelerParticles.at(i)){
			//feelerParticles.at(i)->setPosition(intersection);
			float t1 = (intersection.X - mynodep->getPosition().X); 
			t1 *= t1; 
			float t2 = (intersection.Z - mynodep->getPosition().Z);
			t2*=t2;
			
			s1d->feelerDistances[i] = sqrt(t2 + t1);

			s1d->triangle[i] = triangle;
				
			//printf("%f\n", s1d->feelerDistances[i]);

			//if(ENABLE_DEBUG_OUTPUT){
				//feelerParticles.at(i)->setVisible(true);
				//feelerParticles.at(i)->setText(stringw(
				//(int)s1d->feelerDistances[i]
				 
				//).c_str());
				 //}else{
				//	 feelerParticles.at(i)->setVisible(false);
				 //}
				 
			 //}
		}
		else{
			//if(feelerParticles.at(i))feelerParticles.at(i)->setVisible(false);
			s1d->feelerDistances[i] =s1d->maxRange;
		}
	}
}



void Agent::updateProximitySensor(){
	//Clear all previous entries
	s2d.clear();

	//Run sensor and store values in vector
	this->proximitySensor(1000.0);
}

void Agent::updatePieSensor(){
	//Clear all previous entries
	pie->clear();

	//Run sensor and store values in array
	Agent::PieDetect();
	Agent::showPieSensor();
}


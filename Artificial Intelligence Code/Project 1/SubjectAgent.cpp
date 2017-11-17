#include "SubjectAgent.h"


#include "InputHandler.h"

#include <cmath>

using namespace irr;
using namespace irr::core;


//extern bool ENABLE_DEBUG_OUTPUT;

inline double degreesToRadians(double degrees){
return 2*3.14159*degrees/360;
}

//SubjectAgent::SubjectAgent(irr::scene::IAnimatedMeshSceneNode* a, irr::core::vector3df p):Agent(a,p){
//moving = false;
//}



SubjectAgent::SubjectAgent (Model m, irr::core::vector3df p, irr::scene::ISceneManager* mgr):Agent(m,p,mgr){
	moving = false;
	//feelerParticles = new std::vector<scene::IBillboardSceneNode*>(s1d->getNumFeelers());
	
    //if(s1d->getNumFeelers()!=3)exit(1);

	for(int i = 0; i < s1d->getNumFeelers();i++){
		//
		irr::scene::IBillboardTextSceneNode* a = mgr->addBillboardTextSceneNode(0,L"INF");
		feelerParticles.push_back(a);	

		//mgr->addBillboardTextSceneNode(0,L"ZOMG");

		
	//mgr->addBillboardSceneNode()
	//a->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	// feelerParticles[i]->setMaterialTexture(0,driver->getTexture("../media/particle.bmp"));
	//a->setMaterialFlag(video::EMF_LIGHTING, false);
	a->setMaterialFlag(video::EMF_ZBUFFER,false);
	a->setSize(core::dimension2d<f32>(20.0f, 20.0f));
	a->setVisible(false);

	
	
	}

	for(unsigned int i = 0; i < feelerParticles.size();i++){
		//(*feelerParticles).at(i)->setVisible(false);
		if(feelerParticles.at(i))
		feelerParticles.at(i)->setVisible(false);
	}


	
}



void SubjectAgent::update(irr::ITimer* timer){
    

	Agent::update(timer);

	static const irr::f32 SPEED = .3f;
	static const float ROTATION_RATE =  .10f;
	int TIMEELAPSED =0; //ADJFHKJHFKJHFKJHF
	InputHandler* ih= InputHandler::getInstance();
	
	if(ih==NULL)return;
	if(!mynodep)return;
	
	if(timer==NULL)return;
	

	irr::u32 ctime= 0;
	TIMEELAPSED = (ctime = timer->getTime()) - LASTUPDATE;
	LASTUPDATE = ctime;
	
	irr::core::vector3df nodePos = mynodep->getPosition();
	

	if(ih->isWKeyPressed()){
		//MOVE_FORWARD
		//orientation = mynodep->getRotation().Y;

		//irr::core::vector3df 
			displacement = irr::core::vector3df((irr::f32)cos(degreesToRadians(orientation)), 0.0f, (irr::f32)sin(degreesToRadians(orientation)))* SPEED * (irr::f32)TIMEELAPSED;
//displacement.Z*=-1;			
		nodePos+=displacement;

		mynodep->setPosition(nodePos);

		mynodep->setAnimationSpeed(40);
		if(!moving){moving=true;
			mynodep->setMD2Animation(scene::EMAT_RUN);
		}
		
		position = mynodep->getPosition();
		return;
	}

	if(ih->isAKeyPressed()){
		mynodep->setAnimationSpeed(30);
		//ROTATE_LEFT
		orientation += ROTATION_RATE * TIMEELAPSED;
		mynodep->setRotation(irr::core::vector3df(0.0f,(irr::f32)fabs(360.0f-orientation),0.0f));
		
		//mynodep->setRotation(irr::core::vector3df(0.0f,1,0.0f));
 
		
		if(!moving){moving=true;
			mynodep->setMD2Animation(scene::EMAT_RUN);
			
		}
		//MOVE_LEFT
		//nodePos += irr::core::vector3df((irr::f32)cos(orientation - 90.0f), 0.0f, (irr::f32)sin(orientation - 90.0f))* SPEED * (irr::f32)TIMEELAPSED;
		//mynodep->setPosition(nodePos);
		position = mynodep->getPosition();
return;
	}

	if(ih->isSKeyPressed()){
		//MOVE_BACK
mynodep->setAnimationSpeed(25);
		
    //orientation = mynodep->getRotation().Y;
	//irr::core::vector3df 
		displacement = irr::core::vector3df((irr::f32)cos(degreesToRadians(orientation)), 0.0f, (irr::f32)sin(degreesToRadians(orientation)))* SPEED * (irr::f32)TIMEELAPSED;
	//displacement.Z*=-1;
		nodePos -= displacement/4;//irr::core::vector3df((irr::f32)-cos(orientation), 0.0f, (irr::f32)-sin(orientation))* SPEED * (irr::f32)TIMEELAPSED;
	
	mynodep->setPosition(nodePos);

	
		if(!moving){moving=true;
		mynodep->setMD2Animation(scene::EMAT_RUN);
				
		}
		position = mynodep->getPosition();
return;
	}

	if(ih->isDKeyPressed()){
		//ROTATE_RIGHT

		orientation -= ROTATION_RATE * TIMEELAPSED;
		mynodep->setRotation(irr::core::vector3df(0.0f,(irr::f32)fabs(360.0f-orientation),0.0f));
		
		
		//STRAFE_RIGHT
		//nodePos += irr::core::vector3df((irr::f32)cos(orientation + 90.0f), 0.0f, (irr::f32)sin(orientation + 90.0f))* SPEED * (irr::f32)TIMEELAPSED;
		//mynodep->setPosition(nodePos);

		mynodep->setAnimationSpeed(30);
		if(!moving){moving=true;
			mynodep->setMD2Animation(scene::EMAT_RUN);
			
		}
		
		position = mynodep->getPosition();
return;
	}


	if(moving){moving = false;
	
//		if(!moving){moving=true;
			mynodep->setMD2Animation(scene::EMAT_STAND);
//		}
	}


		position = mynodep->getPosition();



}




void SubjectAgent::updateWallSensor(){

 


 core::line3d<f32> line;
 core::vector3df intersection;
 core::triangle3df triangle;
 core::vector3df orientVector;

 orientVector = core::vector3df((float)cos(degreesToRadians(orientation)),0.0f,(float)sin(degreesToRadians(orientation)));
 line.start = mynodep->getPosition();
 line.end = line.start + orientVector * s1d->maxRange;
 
	//	billboard->setVisible(true);
		 
//std::vector<Agent*> a;
//a = *agentList;
//(*agentList)[0] = NULL;
// a[0] = NULL;
 
 double  baseAngle = -.5f*s1d->getAngle();
 
 baseAngle +=orientation;
 
 double  increment = s1d->getAngle() / (double)s1d->getNumFeelers();

		//	printf("ori%f\n", orientation);
		////	printf("inc%f\n", increment);
		//	printf("getA%f\n", s1d->getAngle());
		//	printf("ba%f\n", baseAngle);
		//	printf("%f\n", 0.0f-90.0f/2.0);


//printf("%d\n",s1d->getNumFeelers()); 
		 for(int i = 0; i < s1d->getNumFeelers(); i++){
				
			double angle = i * increment + baseAngle;
			
			//printf("%f\n", angle);
			
			core::vector3df feelerVector = core::vector3df((float)cos(degreesToRadians(angle)), 0.0f, (float)sin(degreesToRadians(angle)));

			 float t1 = 0.0f;
			 
			 line.start = mynodep->getPosition();
			 line.end = line.start + feelerVector * s1d->maxRange;
			 //printf("s1dmr%d\n", s1d->maxRange();
 
			 if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
				 
				 if(feelerParticles.at(i)){
				 feelerParticles.at(i)->setPosition(intersection);


					 
				 float t1 = (intersection.X - mynodep->getPosition().X); 
				 t1 *= t1; 
				 float t2 = (intersection.Z - mynodep->getPosition().Z);
				 t2*=t2;
				 
				
				 
				 s1d->feelerDistances[i] = sqrt( t2+ t1);
				
				// printf("%f\n", s1d->feelerDistances[i]);

				 if(true){
				 feelerParticles.at(i)->setVisible(true);
				 feelerParticles.at(i)->setText(stringw(
					 (int)s1d->feelerDistances[i]
				 
				 ).c_str());
				 }else{
					 feelerParticles.at(i)->setVisible(false);
				 }
				 
				 }
				 
			 }
		    else{

				//if(feelerParticles.at(i))feelerParticles.at(i)->setVisible(false);
			 s1d->feelerDistances[i] =s1d->maxRange;
			 
			}
		 }
		// printf("//////////////////\n");
}
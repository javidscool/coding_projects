#ifndef SUBJECTAGENT
#define SUBJECTAGENT

#include "irrlicht.h"
#include "Agent.h"

#include <vector>

class SubjectAgent:public Agent{


protected:
bool moving;
std::vector<irr::scene::IBillboardTextSceneNode*> feelerParticles;

public:
virtual void update(irr::ITimer*);
//SubjectAgent(irr::scene::IAnimatedMeshSceneNode* n, irr::core::vector3df p = irr::core::vector3df(0.0f,0.0f,0.0f));



SubjectAgent(Model m, irr::core::vector3df p = irr::core::vector3df(0.0f,0.0f,0.0f),irr::scene::ISceneManager* mgr = NULL);


virtual void updateWallSensor();

};
#endif
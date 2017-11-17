#ifndef MAPGRAPH
#define MAPGRAPH

#include "irrlicht.h"
#include "doubleArray.h"

#include <iostream>
#include <limits>
#include <vector>
#include <string>

using namespace irr;
using namespace irr::core;

class mapGraph{
public:
	
	//graph data
	std::vector<irr::core::vector3df> NODE_VECTOR; // a vector containing the position of each node
	bool** adjacencyList;  //a double array containing whether edge i,j exists
	std::vector<irr::scene::ISceneNode*> SCENE_NODE_VECTOR; //a vector containing a vector of sphere scene nodes representing the graph
	double** costList;//a double array of doubles representing the cost of the edge i,j.  This is usually distance
	bool ENABLE_DEBUG_OUTPUT;
	scene::ISceneManager* smgr;
	irr::scene::ITriangleSelector* selector;
	//void output();

	std::string exclude;
	std::string edges;
	std::string nodes;
	int mapGraph::getClosestNodeUnobstructedSpannable(irr::core::vector3df pos, irr::scene::ISceneManager*, irr::scene::ITriangleSelector* selector);

	mapGraph(irr::scene::ISceneManager*, int numNodes);
	//constructor, loads in the graph from 3 files representing the nodes, the adjacency list, and any edges to exclude from the graph
	mapGraph(IrrlichtDevice *device,const char* nodes, const char* edges, const char* exclude);  
	
	int getClosestNode(irr::core::vector3df pos); //returns the id of the node in the graph with the closest straight line distance to the input position
	int getClosestNodeUnobstructed(irr::core::vector3df pos, irr::scene::ISceneManager*, irr::scene::ITriangleSelector* selector);
	
	//returns a pointer to a vector of integer node id's representing the path from src to target
	std::vector<int>* astarSearch(unsigned int src, unsigned int tgt);
	std::vector<int>* depthFirstSearch(unsigned int src);
	mapGraph* minimumSpanningTree(int src);

	~mapGraph();
	
	inline bool isDebugOutput(){return ENABLE_DEBUG_OUTPUT;};

	inline irr::core::vector3df nodePosition(int i){return NODE_VECTOR[i];}
	
	void toggleDebugOutput(bool);
	void toggleScoreOutput(bool);
	void render(video::IVideoDriver* driver);
	void addNode(irr::core::vector3df);
	void output();

};

#endif
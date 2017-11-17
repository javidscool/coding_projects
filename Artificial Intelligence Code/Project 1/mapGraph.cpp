#include "mapGraph.h"
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <ctime>
#include <cstdlib>

using namespace irr;
using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;
using namespace std;

//#define NODE_MESH_GENERATOR

//returns the closest unobstructed node such that it will be part of the minimum spanning tree: that is, has at least one two sided edge associated with it 
int mapGraph::getClosestNodeUnobstructedSpannable(irr::core::vector3df pos, irr::scene::ISceneManager*, irr::scene::ITriangleSelector* selector){
	
	core::line3d<f32> line;
	core::vector3df intersection;
	core::triangle3df triangle;
	line.start = pos;

	bool* spannable = new bool[NODE_VECTOR.size()];

	for(int i = 0; i < NODE_VECTOR.size(); i++){
		bool tbool;
		tbool = false;
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			if(adjacencyList[i][j] && adjacencyList[j][i] && i != j){
				tbool = true;
				break;
			}
		}
		
		spannable[i] = tbool;
	}



	int sNode1=-1;
	double closest = std::numeric_limits<double>::max();
	line.start = pos;
	for(unsigned int i = 0; i < NODE_VECTOR.size();i++){
		if((pos - NODE_VECTOR[i]).getLength() < closest){
			line.end = NODE_VECTOR[i];
			if(!smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
				if(spannable[i]){
					closest = (pos - NODE_VECTOR[i]).getLength();
					sNode1 = i;
				}
			}
		}
	}

	std::cout<<"the closest is"<<sNode1<<std::endl;

	delete [] spannable;
	return sNode1;
}
	

int mapGraph::getClosestNodeUnobstructed(irr::core::vector3df pos, irr::scene::ISceneManager*, irr::scene::ITriangleSelector* selector){
	core::line3d<f32> line;
	core::vector3df intersection;
	core::triangle3df triangle;
	line.start = pos;


	int sNode1 = -1;
	double closest = std::numeric_limits<double>::max();
	line.start = pos;
	for(unsigned int i = 0; i < NODE_VECTOR.size();i++){
		if((pos - NODE_VECTOR[i]).getLength() < closest){
			line.end = NODE_VECTOR[i];
			if(!smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){
				closest = (pos - NODE_VECTOR[i]).getLength();
				sNode1 = i;
			}
		}
	}
	
	return sNode1;
}
	

//gets the node from the graph closest to the position passed in
int mapGraph::getClosestNode(irr::core::vector3df pos){
	int rn=-1;
	double closest = std::numeric_limits<double>::max();
	for(unsigned int i = 0; i < NODE_VECTOR.size();i++){
		if((pos - NODE_VECTOR[i]).getLength() < closest){
			rn = i;	
			closest = (pos - NODE_VECTOR[i]).getLength();			
		}
	}
	return rn;
}



//performs an a* search from src to tgt, returning the path as a vector of integers representing node numbers
std::vector<int>* mapGraph::astarSearch(unsigned int src, unsigned int tgt){
	using namespace std;
	std::vector<int>* solution = new std::vector<int>;
	if(src > NODE_VECTOR.size() || tgt > NODE_VECTOR.size())
		return solution;

	//solution->push_back(src);
	
	if(src!=tgt){
		bool *visitedArray = new bool[NODE_VECTOR.size()];
		int *parentArray   = new int[NODE_VECTOR.size()];
		double *costArray  = new double[NODE_VECTOR.size()];//cost from each 
		std::vector<int> shortestPathList;
		
		for(unsigned int i = 0; i < NODE_VECTOR.size(); i++){
			visitedArray[i] = false; //not on the shortest path tree
			parentArray[i] = -1; //no parent from the source to node i
			costArray[i] = std::numeric_limits<double>::max(); //the cost to every node from the source is infinite
		}
	
		parentArray[src] = src; //the source node's parent is itself on the path
		//visitedArray[src] = true; //add the source to the shortest path tree
		costArray[src]=0.0f; //because the cost to the node from itself is 0.0
	
		//while the target hasn't been added to the shortest path tree
		while(!visitedArray[tgt]){
			//go through the edges 
			double shortest = std::numeric_limits<double>::max();
			int tNode = -1;
			//int tParent = 0;
		
			for(unsigned int i = 0; i < NODE_VECTOR.size();i++){
				if(!visitedArray[i] && (costArray[i] +  (NODE_VECTOR[tgt] - NODE_VECTOR[i]).getLength())<shortest){
					shortest = costArray[i];
					tNode = i;
				}
			}

			if(tNode == -1){
				solution->clear(); 
				return solution;
			}
			
			visitedArray[tNode] = true;
			shortestPathList.push_back(tNode);

			//look at the new explored node's edges going to any unexplored node and see if we can get a new shortest path 
			for(unsigned int i = 0; i < NODE_VECTOR.size(); i++){
				if(adjacencyList[tNode][i] && visitedArray[i] == false){
					if(costArray[i] > (costArray[tNode] + costList[tNode][i])){
						costArray[i] = (costArray[tNode] + costList[tNode][i]);
						parentArray[i] = tNode;
					}
				}
			}
		}//end while


		solution->push_back(tgt);
		int q = tgt;
		while(q != parentArray[q]){
			solution->push_back(parentArray[q]);
			q = parentArray[q];
		}

		delete[] costArray;
		delete[] visitedArray;
		delete[] parentArray;
	}//end if

	//for(unsigned int i = 0; i < solution->size(); i++){
		//<<(*solution)[i]<<' ';
	//}cout<<'\n';
	
	return solution;
}


void mapGraph::output(){
	#ifdef NODE_MESH_GENERATOR
	//output node mesh files
	core::line3d<f32> line;
	core::vector3df intersection;
	core::triangle3df triangle;

	FILE *fp = fopen(nodes.c_str(), "w");
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		fprintf(fp, "%f %f %f\n", NODE_VECTOR[i].X, NODE_VECTOR[i].Y, NODE_VECTOR[i].Z);
	}
	fclose(fp);
	//std::cout<<"From node:"<< <<"To Node:"<< << std::endl;			
	fp = fopen(edges.c_str(), "w");
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			line.start = NODE_VECTOR[i];
			line.end = NODE_VECTOR[j];
			if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector,intersection, triangle)){	
				std::cout<<'\a';
			}
			else{
				fprintf(fp, "%d %d\n", i, j);
			}
		}
	}
	fclose(fp);
	#endif
}


mapGraph::~mapGraph(){
	std::cout<<"GRAPH DESTRUCTOR\n";
	if(selector == NULL || edges.length() == 0){
		delete[] adjacencyList;
		delete[] costList;
		
		for(unsigned int i = 0; i < SCENE_NODE_VECTOR.size(); i++){
			SCENE_NODE_VECTOR[i]->drop();
		}
	}
	
	output();
	delete[] adjacencyList;
	delete[] costList;

	for(unsigned int i = 0; i < SCENE_NODE_VECTOR.size(); i++){
		SCENE_NODE_VECTOR[i]->drop();
	}
}


mapGraph::mapGraph(IrrlichtDevice *device,const char *n, const char *e, const char *ex):nodes (n), edges (e), exclude (ex){
	smgr = device->getSceneManager();

	ENABLE_DEBUG_OUTPUT = true;

	//load in the adjacency list
	FILE *fp = fopen(nodes.c_str(), "r");
	if(!fp){
		throw new std::string("COULD NOT OPEN MAP GRAPH FILE");
	}
	float a[3];
	while(!feof(fp)){
		fscanf(fp, "%f %f %f\n", a, &a[1], &a[2]);
		NODE_VECTOR.push_back(irr::core::vector3df(a[0],a[1],a[2]));

		scene::ISceneNode * node = smgr->addSphereSceneNode();
		node->setPosition(vector3df(a[0],a[1],a[2]));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		SCENE_NODE_VECTOR.push_back(node);
		node->setVisible(true);

		irr::scene::IBillboardTextSceneNode* a = 
		smgr->addBillboardTextSceneNode(0,stringw(
		(NODE_VECTOR.size()-1)).c_str());
		
		a->setPosition(irr::core::vector3df(0.0f, 15.0f,0.0f));
		//a->setPosition(mynodep->getPosition());
		node->addChild(a);
		a->setMaterialFlag(irr::video::EMF_ZBUFFER,true);
		a->setSize(irr::core::dimension2d<f32>(10.0f, 10.0f));
		node->setVisible(ENABLE_DEBUG_OUTPUT);	
	}
 
	fclose(fp);

	int k = NODE_VECTOR.size();

	//creates adjacency list
	adjacencyList = newDoubleArray<bool>(k,k);
	zero2dArray<bool>(adjacencyList, k, k);

	fp = fopen(edges.c_str(), "r");
	while(!feof(fp)){
		int a, b;
		fscanf(fp,"%d %d\n",&a,&b);
		adjacencyList[a][b] = true;
	}

	fclose(fp);

	//manually remove certain edges
	fp = fopen(exclude.c_str(), "r");
	while(!feof(fp)){
		int a, b;
		fscanf(fp,"%d %d\n",&a,&b);
		adjacencyList[a][b] = false;
	}

	fclose(fp);

	//creates the array of costs for each edge
	costList = newDoubleArray<double>(k,k);

	for(int i = 0; i <k; i++){
		for(int j = 0; j < k; j++){
			costList[i][j] = (NODE_VECTOR[i]- NODE_VECTOR[j]).getLength();
		}
	}


	for(int i = 58; i < 70; i++){
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			if(adjacencyList[i][j]){
				std::cout<<"From"<<i<<":"<<j<<" ";
			}
		}
	}


	//depthFirstSearch(0);
}




void mapGraph::render(video::IVideoDriver* driver){
	if(ENABLE_DEBUG_OUTPUT){
		//then draw lines to show the node mesh graph		
		SMaterial m; 
		m.Lighting=false; 
		m.ZBuffer = 1;
		driver->setMaterial(m); 	
		SColor col;
		for(unsigned int i = 0; i < NODE_VECTOR.size(); i++){
			for(unsigned int j = 0; j < NODE_VECTOR.size(); j++){
				if(adjacencyList[i][j]){
					driver->setTransform(video::ETS_WORLD,SCENE_NODE_VECTOR[i]->getAbsoluteTransformation());
					if(adjacencyList[j][i] && adjacencyList[i][j]){
						col =  SColor(0,255,0,0);
					}
					else{
						col = SColor(255,0,0,255);
					}
					driver->draw3DLine(vector3df(0,0,0), NODE_VECTOR[j]-NODE_VECTOR[i], col);
				}
			}
		}
	}//don't draw lines if not in debug output mode;
}


void mapGraph::toggleDebugOutput(bool b){
	ENABLE_DEBUG_OUTPUT = b;
	
	for(unsigned int i = 0; i < SCENE_NODE_VECTOR.size();i++){
		SCENE_NODE_VECTOR[i]->setVisible(b);
	}
}

void mapGraph::addNode(irr::core::vector3df pos){
	
	//add a node to the list
	NODE_VECTOR.push_back(pos);
	scene::ISceneNode * node = smgr->addSphereSceneNode();
	node->setPosition(pos);
	node->setMaterialFlag(video::EMF_LIGHTING, false);
}


std::vector<int>* mapGraph::depthFirstSearch(unsigned int src){
	bool* visitedArray = new bool[NODE_VECTOR.size()];
	
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		visitedArray[i] = false;
	}
	
	std::vector<int>* solution = new std::vector<int>;
	std::stack<int> sourceNode;
	sourceNode.push(src);

	std::vector< std::list<int> > possibleEdges(NODE_VECTOR.size());

	visitedArray[src]=true;
	srand(time(0));

	//create an edge list
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			if(this->adjacencyList[i][j]){
				possibleEdges[i].push_back(j);
			}
		}
	}

	solution->push_back(src);

	unsigned int random;
	std::list<int>::const_iterator i;

	while(!sourceNode.empty()){
		//select a random edge
		if(!possibleEdges[sourceNode.top()].empty()){
			int tmp;
			while(true){
				if(possibleEdges[sourceNode.top()].size()==0)
					break;
				random = rand();
				random %=possibleEdges[sourceNode.top()].size();
			
				i = (possibleEdges[sourceNode.top()].begin());
				for(int p = 0; p < random; p++){
					i++;
				}
				tmp = *i;
				//std::cout<<"to:"<<*i<<std::endl;

				possibleEdges[sourceNode.top()].remove(tmp); //remove this from the list of possible edges
				
				if(visitedArray[tmp]){
					//std::cout<<"visited edge\n";
					continue;
				}
				else
					break;
			}
			
			//std::cout<<"done getting edge\n";
		
			sourceNode.push(tmp); //go there next 
		    visitedArray[tmp] = true;
			solution->push_back(tmp);
		}
		else{
			sourceNode.pop();
			if(!sourceNode.empty()){
				solution->push_back(sourceNode.top());
			}
		}
	}
	
	delete [] visitedArray;
	return solution;
}


//returns a mapgraph representing the minimum spanning tree 
mapGraph* mapGraph::minimumSpanningTree(int src){
	bool* visitedArray = new bool[NODE_VECTOR.size()];
	std::list<int>::const_iterator iter;
	
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		visitedArray[i] = false;
	}
	
	mapGraph* minGraph = new mapGraph(smgr, NODE_VECTOR.size());
	
	//std::cout<<"created private graph\n";
	minGraph->NODE_VECTOR  = this->NODE_VECTOR;
	minGraph->SCENE_NODE_VECTOR = this->SCENE_NODE_VECTOR;

	//copy the cost array from this to the mingraph
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		for(int j = 0; j < NODE_VECTOR.size();j++){
			minGraph->costList[i][j] = this->costList[i][j];
		}
	}

	//next use prim's algorithm to get the adjacencyList of the new graph
	visitedArray[src] = true;
	int visitedCount = 1;

	//create a table of all the edges in the graph that we can remove from to save search time
	std::vector<std::list<int> > possibleEdges(NODE_VECTOR.size());


	//create an edge list
	for(int i = 0; i < NODE_VECTOR.size(); i++){
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			if(this->adjacencyList[i][j] 
			   && this->adjacencyList[j][i]//HACKHACKHACKHACK
			   ){
				   possibleEdges[i].push_back(j);
			   }
		}
	}


	int src=-1, tgt=-1;
	double shortest;
	//then look at each node in the graph, and look at its edges going to an unvisited node.  add in the edge not 
	while(visitedCount < NODE_VECTOR.size()){//loop until all the nodes in the graph are in the minimum spanning tree.
		shortest= std::numeric_limits<double>::max();
		src = -1;
		tgt = -1;

		//look at each visited node in the graph
		for(int i = 0; i < NODE_VECTOR.size();i++){
			if(!visitedArray[i])
				continue;
			if(possibleEdges[i].empty())
				continue; //if there's no possible edges from this node continue
			iter = possibleEdges[i].begin();

			//std::cout<<"got the beginning iterator\n";

			//look at all the edges going out of this node	
			for(int j = 0; j < possibleEdges[i].size(); j++){
				int tmp;
				tmp = *iter; //get the value pointed to by the iterator 

				//if the edge goes to a visited node remove it from the edge list
				if(visitedArray[tmp]){
					//possibleEdges[i].remove(tmp); //this won't work?
				}
				else{
					if(this->costList[i][tmp] < shortest){
						shortest = this->costList[i][tmp];
						src = i;
						tgt = tmp;
					}
				}

				iter++;
		
				//std::cout<<"incrementing iterator\n";
			}
		}//end double for loop through edges

		//now that we've looped through all the edges add in the edge to the new graph that's the lowest cost such that edge(i,j) 
		//is the minimum edge where i is already in the graph and j isn't

		if(src < 0 || tgt < 0)
			break;
		visitedArray[tgt] = true;
		visitedCount++;
		minGraph->adjacencyList[src][tgt] = true;
		minGraph->adjacencyList[tgt][src] = true;
		//std::cout<<"adjusted list\n";
	}//end :while a node isn't in the graph
	//std::cout<<"got to end of function\n";

	for(int i = 0; i < NODE_VECTOR.size(); i++){
		for(int j = 0; j < NODE_VECTOR.size(); j++){
			if(minGraph->adjacencyList[i][j]){
				//std::cout<<i<<":"<<j<<std::endl;
			}
		}
	}
	
	return minGraph;
}


mapGraph::mapGraph(irr::scene::ISceneManager* mgr, int numNodes){
	
	smgr = mgr;
	adjacencyList = newDoubleArray<bool>(numNodes,numNodes);
	zero2dArray<bool>(adjacencyList, numNodes, numNodes);

	costList = newDoubleArray<double>(numNodes,numNodes);
}
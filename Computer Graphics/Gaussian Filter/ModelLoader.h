#ifndef MODELLOADER_H_
#define MODELLOADER_H_
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "GL/glew.h"
#include "GL/glut.h"

struct myVertex
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

bool loadModel(char *fileName, myVertex* &vertices, int &num_verts);


#define NUM_BUFFERS 1
class CModel
{
public:

	GLuint	buffer_name;	// Vertex buffer ID
	myVertex *vertices;		// List of vertices
	int num_verts;			// Number of vertices

	~CModel();				// Destructor

	// Use thse functions
	bool loadModel(char *fileName);		// Load Model
	void render();						// Send Vertex positions, normals, etc for rendering
};

#endif
#include "ModelLoader.h"
using std::cout;
using std::endl;


// File loader
bool loadModel(char *fileName, myVertex* &vertices, int &num_verts)
{
	FILE *fp = NULL;
	fp = fopen(fileName, "rb");
	if(!fp) return false;

	int _nverts = -1;
	int _nfaces = -1;

	fscanf(fp, "%i", &_nverts);
	if(_nverts <1)
	{
		fclose(fp);
		return false;
	}

	myVertex *verts = new myVertex[_nverts];

	for(int i=0;i<_nverts;i++)
		fscanf(fp, "%f %f %f %f %f %f %f %f", &verts[i].x, &verts[i].y, &verts[i].z, &verts[i].nx, &verts[i].ny, &verts[i].nz, &verts[i].s, &verts[i].t);

	fscanf(fp, "%i", &_nfaces);
	if(_nfaces < 1)
	{
		delete [] verts;
		fclose(fp);
		return false;
	}
	vertices = new myVertex[_nfaces * 3];
	num_verts = _nfaces * 3;

	for(int i=0;i<_nfaces;i++)
	{
		int x, y, z;
		fscanf(fp, "%i %i %i", &x, &y, &z);
		vertices[i*3 + 0] = verts[x];
		vertices[i*3 + 1] = verts[y];
		vertices[i*3 + 2] = verts[z];
	}

	fclose(fp);
	return true;
}


// CModel functions

// clean up
CModel::~CModel()
{
	glDeleteBuffers(NUM_BUFFERS,&buffer_name);

	if(this->num_verts>0)
		delete [] this->vertices;
	this->num_verts = 0;
}


// Load model and create Vertex Buffer Objects
bool CModel::loadModel(char *fileName)
{
	if(!::loadModel(fileName, vertices, num_verts))
	{
		cout << "Could not load model file" << endl;
		return false;
	}

	// generate buffer name
	glGenBuffers(NUM_BUFFERS, &buffer_name);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_name);
	// set buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(myVertex)*num_verts, this->vertices, GL_STATIC_DRAW);

	return true;
}

void CModel::render()
{
	// enable vertex positions and normals
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// bind budffer
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_name);
	// specify where the positions/normals are in the structure
	glVertexPointer(3, GL_FLOAT, sizeof(myVertex), NULL);
	glNormalPointer(GL_FLOAT, sizeof(myVertex), (unsigned char*)(NULL) + sizeof(float)*3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(myVertex), (unsigned char*)(NULL) + sizeof(float)*6);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, this->num_verts);


	// disable vertex positions and normals
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
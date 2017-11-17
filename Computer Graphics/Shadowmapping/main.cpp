#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include "fbo.h"
using std::cout;
using std::endl;
#include "glsl.h"
#include "ModelLoader.h"
#include "TextureLoader.h"

// shader programs
GLuint prog_id;
GLuint prog_id2;

// 3D model
CModel myModel;
CModel myModel2;

// frame buffer object
FBO* myFBO;

// render target texture
CTexture* myRT;

// store width and height
int g_width;
int g_height;

// light matrices
float lightV[16];	// light's view matrix
float lightP[16];	// light's projection matrix
float lightTigerMV[16]; //used for getting tiger's model view information
float lightFloorMV[16]; //used for getting floor's model view information
float lightWallMV[16]; //used for getting wall's model view transformation

// camera matrices
float cameraV[16];	// camera's view matrix
float cameraW[16];	// camera's world matrix

// Rendering function
void render()
{
	//offset so tiger can rotate by 0.5 degrees every frame
	static float offset = 0.0;
	if(offset > 360.0){
		offset = 0.0;
	}
	//offsetting by 0.5 degrees
	offset += 0.5;

	// Render to texture
	glUseProgram(prog_id2);
	myFBO->beginRenderToTexture();

	// clear render target texture
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection transformation 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(myRT->width)/myRT->height, 0.1, 100.0);
	// get the light's projection matrix and send it to shader as uniform
	glGetFloatv(GL_PROJECTION_MATRIX, lightP);
	
	// View transformation parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,7, 0,0,0, 0,1,0);
	// get the light's view matrix and send it to the shader as uniform
	glGetFloatv(GL_MODELVIEW_MATRIX, lightV);

	// render tiger
	glPushMatrix();
	glRotatef(offset, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	//setting the model view matrix for the tiger to lightTigerMV
	glGetFloatv(GL_MODELVIEW_MATRIX,lightTigerMV);
	myModel2.render();
	glPopMatrix();

	// render floor
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 1.0f);
	glScalef(5.0, 5.0, 5.0);
	//setting the model view matrix for the floor to lightFloorMV
	glGetFloatv(GL_MODELVIEW_MATRIX,lightFloorMV);
	//commented out cause floor is not casting shadow in this scene
	//myModel.render();
	glPopMatrix();
	
	//render wall
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -4.5f);
	glScalef(5.0, 5.0, 5.0);
	//setting the model view matrix for the wall to lightWallMV
	glGetFloatv(GL_MODELVIEW_MATRIX,lightWallMV);
	glPopMatrix();

	//fix for outputting shadow map to a file
	//I commented this out cause it slows down your rotation big time (but I knew the fix, so I want my 5%)
	//myRT->pixels = new unsigned char[3*256*256];
	//glReadPixels(0,0,256,256,GL_RGB,GL_UNSIGNED_BYTE,myRT->pixels);
	//myRT->savePPM("shadowmapoutput.ppm");

	myFBO->endRenderToTexture();


	// Render to back buffer
	glUseProgram(prog_id);
	// send the light's view and projection matrices
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"lightP"), 1, false, lightP);
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"lightV"), 1, false, lightV);

	//sending the model view matrix of the tiger to the shader
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"lightTigerMV"), 1, false, lightTigerMV);

	//sending the model view matrix of the floor to the shader
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"lightFloorMV"), 1, false, lightFloorMV);

	//sending the model view matrix of the wall to the shader
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"lightWallMV"), 1, false, lightWallMV);

	// Clear the back buffer
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(g_width)/g_height, 0.1, 100.0);

	// View transformation parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1,4,8, 0,0,0, 0,1,0);
	//gluLookAt(2,1,5, 0,0,0, 0,1,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraV);
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"cameraV"), 1, false, cameraV);

	// render tiger
	glUniform1i(glGetUniformLocation(prog_id, "obj"), 1);
	glLoadIdentity();
	glRotatef(offset, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"cameraW"), 1, false, cameraW);
	myModel2.render();

	// render floor
	glUniform1i(glGetUniformLocation(prog_id, "obj"), 0);
	glLoadIdentity();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 1.0f);
	glScalef(5.0, 5.0, 5.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"cameraW"), 1, false, cameraW);
	myModel.render();


	// render wall
	glUniform1i(glGetUniformLocation(prog_id, "obj"), -1);
	glLoadIdentity();
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -4.5f);
	glScalef(5.0, 5.0, 5.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id,"cameraW"), 1, false, cameraW);
	myModel.render();


	// swap buffers (double buffering)
	glutSwapBuffers();
}


// Window resize
void reshape(int width, int height)
{
	g_width = width;
	g_height = height;
	// update viewport to cover the entire window
	glViewport(0, 0, width, height);
}


int main(int argc, char **argv)
{
	// Initialize glut
	glutInit(&argc, argv);	

	// Top left corner of window
	glutInitWindowPosition(10,10);
	// Size of window
	glutInitWindowSize(800,600);
	// Display mode color mode | double buffering
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	// Create the window
	glutCreateWindow("CAP4720");

	glEnable(GL_DEPTH_TEST);

	// Initialize GLEW
	glewInit();
	if(glewIsSupported("GL_VERSION_2_0"))
	{
		cout << "OpenGL 2.0 is supported" << endl;
	}
	else
	{
		cout << "OpenGL 2.0 is not supported" << endl;
		return 0;
	}

	// Create GLSL shaders and program
	prog_id = createGLSLProgram("shaders/simple.vert", "shaders/simple.frag");
	prog_id2 = createGLSLProgram("shaders/simple2.vert", "shaders/simple2.frag");
	glUseProgram(prog_id);


	// Load 3D Model (use the CModel class!!)
	if(!myModel.loadModel("quad.txt"))
	{
		cout << "Could not load model file" << endl;
		return 0;
	}

	// Load another model
	if(!myModel2.loadModel("tigertc.txt"))
	{
		cout << "Could not load model file" << endl;
		return 0;
	}

	// Load textures
	CTexture myTex, myTex1, myTex2, myTex3;
	// Specify that we want to use texture unit 0 for the following texture
	glActiveTexture(GL_TEXTURE0);	
	myTex.loadPPM("tiger.ppm");
	glActiveTexture(GL_TEXTURE2);	
	myTex.loadPPM("brick.ppm");
	glActiveTexture(GL_TEXTURE3);	
	myTex.loadPPM("stainedglass.ppm");
	// Specify that we want to use texture unit 1 for the following texture
	glActiveTexture(GL_TEXTURE1);
	myTex1.loadPPM("poster.ppm");

	// Specify the texture units we are using to the shader
	glUniform1i(glGetUniformLocation(prog_id, "tex1"), 0);
	glUniform1i(glGetUniformLocation(prog_id, "tex2"), 1);
	glUniform1i(glGetUniformLocation(prog_id, "tex3"), 2);
	glUniform1i(glGetUniformLocation(prog_id, "tex4"), 3);

	// Create FBO and render target texture
	myRT = new CTexture();
	myRT->createRT(256,256);
	myFBO = new FBO(*myRT);

	//computing texel size and passing it to simple.frag
	float texelsize;
	texelsize = 1.0 / 256.0;
	glUniform1f(glGetUniformLocation(prog_id, "texsize"), texelsize);

	// Register GLUT callbacks
	// If window is resized, call the reshape() function
	glutReshapeFunc(reshape);
	// For rendering, call the render() function
	glutDisplayFunc(render);
	// Idle function
	glutIdleFunc(render);

	// Keep looping (and rendering) until program exits
	glutMainLoop();

	// save the texture
	delete myRT;
	delete myFBO;

	// After the application has exited, clean up
	cleanupGLSL(prog_id);

	return 0;
}
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
GLuint prog_id3;
GLuint prog_id4;

// 3D model
CModel myModel;
CModel myModel2;

// frame buffer object
FBO* myFBO;

// render target texture
CTexture* myRT;
CTexture* myRT2;
CTexture* myRT3;
CTexture* myRT4;
CTexture* myRT5;
CTexture* myRT6;
CTexture* myRT7;
CTexture* myRT8;

// store width and height
int g_width;
int g_height;

// light matrices
float lightV[16];	// light's view matrix
float lightP[16];	// light's projection matrix

// camera matrices
float cameraV[16];	// camera's view matrix
float cameraW[16];	// camera's world matrix

static float offset = 0.0;

void submitGeometry()
{
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
	myModel2.render();
	glPopMatrix();

	// render floor
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 1.0f);
	glScalef(5.0, 5.0, 5.0);
	myModel.render();
	glPopMatrix();
}

void cameraShit(){
	
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
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraV);
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"cameraV"), 1, false, cameraV);

	// render tiger
	glUniform1i(glGetUniformLocation(prog_id2, "obj"), 1);
	glLoadIdentity();
	glRotatef(offset, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"cameraW"), 1, false, cameraW);
	myModel2.render();

	// render floor
	glUniform1i(glGetUniformLocation(prog_id2, "obj"), 0);
	glLoadIdentity();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 1.0f);
	glScalef(5.0, 5.0, 5.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"cameraW"), 1, false, cameraW);
	myModel.render();


	// render wall
	glUniform1i(glGetUniformLocation(prog_id2, "obj"), -1);
	glLoadIdentity();
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -4.5f);
	glScalef(5.0, 5.0, 5.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraW);
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"cameraW"), 1, false, cameraW);
	myModel.render();


}

// Rendering function
void render()
{
	//setting the clear color for when we clear the render target
	glClearColor(1,1,1,1);
	//clears render targets
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//offset so tiger can rotate by 0.5 degrees every frame
	if(offset > 360.0){
		offset = 0.0;
	}
	//offsetting by 0.5 degrees
	offset += 0.5;

	// Render to texture
	glUseProgram(prog_id);

	myFBO->beginRenderToTexture(0);
	submitGeometry();
	myFBO->endRenderToTexture();


	// Render to back buffer
	glUseProgram(prog_id2);
	// send the light's view and projection matrices
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"lightP"), 1, false, lightP);
	glUniformMatrix4fv(glGetUniformLocation(prog_id2,"lightV"), 1, false, lightV);
	
	//designating a pass variable to denote which pass we're on
	//on pass 1 we're drawing the shadow three times to three different render targets at different resolutions each time
	glUniform1i(glGetUniformLocation(prog_id2, "pass"), 1);
	myFBO->beginRenderToTexture(1);
	cameraShit();
	myFBO->endRenderToTexture();

	myFBO->beginRenderToTexture(2);
	cameraShit();
	myFBO->endRenderToTexture();

	myFBO->beginRenderToTexture(3);
	cameraShit();
	myFBO->endRenderToTexture();
	
	//on pass 2 we're just drawing the tiger and the walls with no shadow
	glUniform1i(glGetUniformLocation(prog_id2, "pass"), 2);
	cameraShit();

	//first bind fullbright image to a render target
	myFBO->beginRenderToTexture(4);
	cameraShit();
	myFBO->endRenderToTexture();


	//**texture units are one greater than their render target counterparts**
	
	//burring the x-axis and y-axis with prog_id4
	glUseProgram(prog_id4);
	//variable for texel size
	float texelsize;
	
	//pass 1 blurs x-axis only
	glUniform1i(glGetUniformLocation(prog_id4, "pass"), 1);
	
	//setting render target 5 to the x-axis blur of temp shadow low
	myFBO->beginRenderToTexture(5);
	//computing texel size for shadow low and passing it to blur.frag
	texelsize = 1.0 / 256.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 2);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();

	//setting render target 6 to the x-axis blur of temp shadow med
	myFBO->beginRenderToTexture(6);
	//computing texel size for shadow med and passing it to blur.frag
	texelsize = 1.0 / 512.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 3);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();

	//setting render target 7 to the x-axis blur of temp shadow high
	myFBO->beginRenderToTexture(7);
	//computing texel size for shadow high and passing it to blur.frag
	texelsize = 1.0 / 1024.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 4);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();


	//pass 2 blurs y-axis only
	glUniform1i(glGetUniformLocation(prog_id4, "pass"), 2);
	
	//setting render target 1 to the y-axis blur of shadow low
	myFBO->beginRenderToTexture(1);
	//computing texel size for shadow low and passing it to blur.frag
	texelsize = 1.0 / 256.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 6);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();

	//setting render target 2 to the y-axis blur of shadow med
	myFBO->beginRenderToTexture(2);
	//computing texel size for shadow med and passing it to blur.frag
	texelsize = 1.0 / 512.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 7);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();

	//setting render target 3 to the y-axis blur of shadow high
	myFBO->beginRenderToTexture(3);
	//computing texel size for shadow high and passing it to blur.frag
	texelsize = 1.0 / 1024.0;
	glUniform1f(glGetUniformLocation(prog_id4, "texsize"), texelsize);
	glUniform1i(glGetUniformLocation(prog_id4, "texture"), 8);
	//clears render targets before we draw over it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//drawing render target after I cleared it above
	myModel.render();
	myFBO->endRenderToTexture();
	


	//multipling the blurs with prog_id3
	glUseProgram(prog_id3);
	//setting fullbright (5 in this case) to tex6
	glUniform1i(glGetUniformLocation(prog_id3, "tex6"), 5);
	//setting shadow-low (2 in this case) to tex3
	glUniform1i(glGetUniformLocation(prog_id3, "tex3"), 2);
	//setting shadow-med (3 in this case) to tex4
	glUniform1i(glGetUniformLocation(prog_id3, "tex4"), 3);
	//setting shadow-high (4 in this case) to tex5
	glUniform1i(glGetUniformLocation(prog_id3, "tex5"), 4);
	//drawing the whole quad which already contains everything to be drawn
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
	prog_id = createGLSLProgram("shaders/build_shadow_map.vert", "shaders/build_shadow_map.frag");
	prog_id2 = createGLSLProgram("shaders/shadow_map.vert", "shaders/shadow_map.frag");
	prog_id3 = createGLSLProgram("shaders/composite.vert", "shaders/composite.frag");
	prog_id4 = createGLSLProgram("shaders/blur.vert", "shaders/blur.frag");


	glUseProgram(prog_id2);

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
	CTexture myTex, myTex1;
	// Specify that we want to use texture unit 0 for the following texture
	glActiveTexture(GL_TEXTURE0);	
	myTex.loadPPM("tiger.ppm");
	// Specify that we want to use texture unit 1 for the following texture
	glActiveTexture(GL_TEXTURE1);
	myTex1.loadPPM("poster.ppm");

	// Specify the texture units we are using to the shader
	glUniform1i(glGetUniformLocation(prog_id2, "tex1"), 0);
	glUniform1i(glGetUniformLocation(prog_id2, "tex2"), 1);


	// Create FBO and render target texture
	myRT = new CTexture();
	myRT->createRT(512,512);

	myRT2 = new CTexture();
	myRT2->createRT(256,256);
	
	myRT3 = new CTexture();
	myRT3->createRT(512,512);
	
	myRT4 = new CTexture();
	myRT4->createRT(1024,1024);

	//creating new render target to render the plain scene with no shadows
	myRT5 = new CTexture();
	myRT5->createRT(800,600);
	
	//myRT6, myRT7, myRT8 are the three temporary render targets that are needed to properly blur the three shadows (which are at different resolutions)	
	myRT6 = new CTexture();
	myRT6->createRT(256,256);

	myRT7 = new CTexture();
	myRT7->createRT(512,512);

	myRT8 = new CTexture();
	myRT8->createRT(1024,1024);


	myFBO = new FBO();
	myFBO->setTexture(0, *myRT);
	myFBO->setTexture(1, *myRT2);
	myFBO->setTexture(2, *myRT3);
	myFBO->setTexture(3, *myRT4);
	myFBO->setTexture(4, *myRT5);
	myFBO->setTexture(5, *myRT6);
	myFBO->setTexture(6, *myRT7);
	myFBO->setTexture(7, *myRT8);


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
	cleanupGLSL(prog_id2);

	return 0;
}
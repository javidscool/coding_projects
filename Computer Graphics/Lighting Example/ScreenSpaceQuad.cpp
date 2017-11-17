#include "ScreenSpaceQuad.h"

//const unsigned int STRIDE = 7*sizeof(GL_FLOAT);



ScreenSpaceQuad::~ScreenSpaceQuad(){
if(copycount.unique()){

	//std::cout<<"Screenspace quad destructor\n";
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &normals);

	glDeleteBuffers(1, &texcoords);


}
}

ScreenSpaceQuad::ScreenSpaceQuad(){
		std::cout<<"Screenspace quad ctor"<<std::endl;

	   //todo refactor into .txt file so this array isn't always in client memory
//		const unsigned int ARRSIZE = 28;
//		const unsigned int SIZE = ARRSIZE*sizeof(GLfloat);

        const GLfloat datanorm[12] = {0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f};

        const GLfloat datatex[8] = {0.0f, 0.0f,1.0f, 0.0f,1.0f, 1.0f,0.0f, 1.0f};

		const GLfloat datavert[12] = {
									//lower left
										  //tex coords

									-1.0f, -1.0f, .50, //verts

									//lower right
									//tex coord

									1.0f, -1.0f, .50,//vert

									//upper right
									//tex coord

									1.0f, 1.0f,.50,//vert

									//upper left
									//tex coord
									//normal
									-1.0f, 1.0f, .50//vert

									};

        glGenVertexArrays(1, & vertex_array);
		glGenBuffers(1, &quadVBO);
        glGenBuffers(1, &normals);
        glGenBuffers(1, &texcoords);

		if(! vertex_array || ! quadVBO || !normals || !texcoords ){
		 throw se::graphics::gl::ResourceAllocationException("unable to allocate ssq vbo/vert array");
		}

        glBindVertexArray(vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *12, datavert, GL_STATIC_DRAW);
		glVertexAttribPointer(0,3, GL_FLOAT, false, 0, 0 );


		glBindBuffer(GL_ARRAY_BUFFER, texcoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, datatex, GL_STATIC_DRAW);
		glVertexAttribPointer(1,2,GL_FLOAT, false, 0, 0);


        glBindBuffer(GL_ARRAY_BUFFER, normals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, datanorm, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);

        //position, tex coord, normal
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

}



void ScreenSpaceQuad::draw(se::graphics::gl::OpenGLRuntime& gl, se::graphics::gl::GLSimpleShaderProgram& shader) const {

//std::cout<<"Screenspace quad draw\n";
//todo: is this necessary? based on the client state enable not being maintained between buffer objects i'd say yes.

shader.bind();

glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "modelmatrix"), 1, GL_FALSE,modelingMatrix.data());
glUniformMatrix3fv(glGetUniformLocation(shader.getID(), "normalmatrix"), 1, GL_FALSE,modelingMatrix.data());

glBindVertexArray(vertex_array);
glBindAttribLocation(shader.getID(), 0, "position");
glBindAttribLocation(shader.getID(), 1, "texcoords0");
glBindAttribLocation(shader.getID(), 2, "normal");

//glLinkProgram(shader.getID());
 /*glUniform1i(glGetUniformLocation(shader.getID(), "normaltexture"), 4);
            glUniform1i(glGetUniformLocation(shader.getID(), "depthtexture"), 5);
*/
glDrawArrays(GL_QUADS, 0, 4);



}

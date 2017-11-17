#ifndef SCREENSPACEQUAD_H_INCLUDED
#define SCREENSPACEQUAD_H_INCLUDED


#include <se/graphics/gl/OpenGLRuntime.h>
#include <iostream>
#include <se/core/CopyCounter.h>
#include <se/graphics/gl/GLSimpleShaderProgram.h>
//todo: make renders take in openglrenderconfig (Steve version) to validate that gl calls are valid
//todo: bindable 3d model class
class ScreenSpaceQuad{

	private:

        GLuint vertex_array;
		GLuint quadVBO;
		GLuint normals;
		GLuint texcoords;

        se::core::CopyCounter copycount;


        Eigen::Matrix4f modelingMatrix;

        Eigen::Matrix3f normalMatrix;

	public:

void setModelMatrix(Eigen::Matrix4f& modelMatrix){
  modelingMatrix = modelMatrix;
}


void setNormalMatrix(Eigen::Matrix3f& nm){
  normalMatrix = nm;
}

        ScreenSpaceQuad();

		void draw(se::graphics::gl::OpenGLRuntime& rt, se::graphics::gl::GLSimpleShaderProgram& sh) const;


		~ScreenSpaceQuad();


};


#include "ScreenSpaceQuad.tcc"

#endif // SCREENSPACEQUAD_H_INCLUDED

#version 150

attribute vec3 position; 
attribute vec2 texcoords0;
attribute vec3 normal; 
attribute vec3 tangent;
attribute vec3 bitangent;

uniform mat4 cameramatrix;
uniform mat4 modelmatrix;
uniform mat3 normalmatrix;

varying vec3 posObjVert;
varying vec3 normVert;


void main(){
float fov = 45.0;
float f = 1.0  / tan(fov / 2.0);
float zFar = 100.0;
float zNear = .01;
float aspect = 4.0 / 3.0;

/*
vec4 wtrans = modelmatrix * vec4(position.xyz,1.0);
worldspaceVert = wtrans.xyz;
gl_Position =cameramatrix * wtrans;
*/

gl_Position = vec4(position, 1.0);

//gl_Position.xyz*=.1;

//gl_Position.xyz -= vec3(0.20,0.20,0);

normVert = normalmatrix * normal;
posObjVert = normal;


}



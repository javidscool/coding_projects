#version 120

attribute vec3 position; 
attribute vec2 texcoords0;
attribute vec3 normal; 
attribute vec3 tangent;
attribute vec3 bitangent;

uniform mat4 cameramatrix;
uniform mat4 modelmatrix;
uniform mat3 normalmatrix;

uniform mat4 projmatrix;

varying vec3 worldPos;
varying vec3 worldNormal;
varying vec2 textureCoords;

void main(){

	//setting the position of the vertex in world space
	vec4 temp = modelmatrix * vec4(position, 1.0);
	worldPos = temp.xyz;

	//setting the position of the normal in world space
	worldNormal = normalmatrix * normal;
	
	textureCoords = texcoords0;
	

	//putting positions of triangle into clip space
	gl_Position = projmatrix * cameramatrix * modelmatrix * vec4(position, 1.0);

}

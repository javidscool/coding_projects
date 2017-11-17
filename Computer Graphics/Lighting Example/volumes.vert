#version 150

attribute vec3 position; 
uniform mat4 modelmatrix;
uniform mat4 cameramatrix;

void main(){

gl_Position =/* cameramatrix * modelmatrix */ vec4(position, 1.0);

}



#version 150

attribute vec3 position;  

uniform mat4 cameramatrix;
uniform mat4 modelmatrix;
 
 varying vec3 pos;


void main(){
 
gl_Position = cameramatrix * modelmatrix * 
vec4(position, 1.0);
  

pos = position;
}



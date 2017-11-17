#version 120

attribute vec3 position;
attribute vec2 texcoords0;

varying vec2 texcoord;

void main(){

  texcoord = texcoords0;
  gl_Position = vec4(position,1.0);


}

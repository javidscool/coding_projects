#version 120



attribute vec3 position; 
attribute vec2 texcoords0;
varying vec2 coords;

void main(){

gl_Position = vec4(position, 1.0);
coords = texcoords0;

}

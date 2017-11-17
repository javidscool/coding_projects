attribute vec3 position;
uniform sampler2D depthtexture;

varying vec4 lightspacepos;
//varying vec4 clipPos;

uniform mat4 cameramatrix;
uniform mat4 modelmatrix;
uniform mat4 lightmatrix;

void main(){

gl_Position = cameramatrix * modelmatrix* vec4(position, 1.0);
lightspacepos = lightmatrix * modelmatrix * vec4(position, 1.0);



}

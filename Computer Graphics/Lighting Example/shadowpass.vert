
attribute vec3 position;
varying vec4 eyeposition;

uniform mat4 lightmatrix;
uniform mat4 modelmatrix;
uniform mat4 projection;

void main(){
/*
float fov = 45.0;
float f = 1.0  / tan(fov / 2.0);
float zFar = 100.0;
float zNear = .01;
float aspect = 4.0 / 3.0;

mat4 pmat = mat4( vec4(f / aspect, 0.0, 0.0, 0.0), 
		vec4(0.0, f, 0.0, 0.0), 
		vec4(0.0, 0.0, (zFar + zNear) / (zNear - zFar), -1.0),
		vec4(0.0, 0.0, 2 * zFar * zNear / (zNear - zFar), 0.0)
);*/


eyeposition = lightmatrix * modelmatrix*  vec4(position.xyz,1.0);
gl_Position =  projection * eyeposition;

}

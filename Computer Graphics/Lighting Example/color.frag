#version 150
uniform bool color;

varying vec3 pos;

void main(){
if(color)
gl_FragColor = vec4(1,0,0,1);

else gl_FragColor = vec4(0,1,0,1);

gl_FragColor = vec4(pos,1.0);

}

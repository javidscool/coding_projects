#version 150

varying vec3 norm;
varying vec3 posObj;
varying vec3 worldspace;

void main(){

//gl_FragColor = vec4(1,0,0,1);
gl_FragData[0] = vec4(norm, 1.0);//vec4(vec3(dot(normalize(norm), vec3(0,0,1))),1);
//gl_FragData[1] = vec4(posObj, 1.0);
gl_FragData[1] = vec4(worldspace, 0.0);

}

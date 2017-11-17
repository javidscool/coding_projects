#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
 

/*
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
*/


varying in vec3 posObjVert[];
varying in vec3 normVert[];
//varying in vec3 worldspaceVert[];


uniform mat4 cameramatrix;
uniform mat4 modelmatrix;
uniform mat3 normalmatrix;
uniform float offset;

varying out vec3 posObj;
varying out vec3 norm;
//varying out vec3 worldspace;

varying out vec3 worldspaceVert;
void main(){

vec3 faceNormal  =   normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));

for(int i = 0; i < 3; i++){


vec4 lpos = gl_in[i].gl_Position;
lpos.xyz += offset * faceNormal;
 
vec4 wtrans = modelmatrix * lpos;
worldspaceVert = wtrans.xyz;
gl_Position =cameramatrix * wtrans;

//gl_Position =  gl_in[i].gl_Position ;
//gl_Position.xyz +=  .05 * faceNormal;

posObj = posObjVert[i];
norm = normVert[i];
//worldspace = worldspaceVert[i];

EmitVertex();

}

EndPrimitive();


}

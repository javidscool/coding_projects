 
#version 150
layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 18) out;
 

uniform mat4 cameramatrix;
uniform mat4 modelmatrix;

uniform mat4 eyemat;

uniform bool zpass;


uniform mat3 normalmatrix;


void main(){
  
 

 mat4 modelview = eyemat * modelmatrix;

vec3 faceNormal[3];



/*
//calculate face normals at each vertex in eye coordinates
faceNormal[0] = cross(

normalmatrix*( vec4(gl_in[2].gl_Position.xyz,1.0)).xyz - normalmatrix*( vec4(gl_in[0].gl_Position.xyz,1.0)).xyz, 
normalmatrix*(vec4(gl_in[4].gl_Position.xyz,1.0)).xyz - normalmatrix*( vec4(gl_in[0].gl_Position.xyz,1.0)).xyz
);*/


//calculate face normals at each vertex in eye coordinates

 
faceNormal[0] = cross(

(modelview*vec4(gl_in[2].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[0].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[4].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[0].gl_Position.xyz,1.0)).xyz
);
 


faceNormal[1] = cross(

(modelview*vec4(gl_in[4].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[2].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[0].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[2].gl_Position.xyz,1.0)).xyz
);

faceNormal[2] = cross(

(modelview*vec4(gl_in[0].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[4].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[2].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[4].gl_Position.xyz,1.0)).xyz
);

 


 
//world space light direction
vec3 lightdir = vec3(0,0,1);

bool faces_light = true;

//light direction in eye space
vec3 eyelight = (eyemat*vec4(lightdir,0.0)).xyz;
eyelight = normalize(eyelight);

//test and see if the triangle is facing the light
if(!(dot( eyelight, faceNormal[0]) > 0.0 
    || dot(eyelight, faceNormal[1]) > 0.0 
   || dot(eyelight, faceNormal[2]) > 0.0
 

)){	

//wtf note:: before, when the else case wasn't inside an else block, the cap was *always* being generated.
//is return inside a branch not valid for geometry shader?

  return; //if it is, do nothing
faces_light = false;
} //the triangle is not facing the light! return

else{
if(faces_light && !zpass ){
  gl_Position =    cameramatrix*modelmatrix*gl_in[0].gl_Position;
	EmitVertex();  

  gl_Position = cameramatrix*modelmatrix*gl_in[2].gl_Position;

	EmitVertex();

  gl_Position =  cameramatrix*modelmatrix*gl_in[4].gl_Position;
	EmitVertex();


EndPrimitive();	

}}
 
 
//extrude all edges of the input triangle if needed
for(int i = 0; i < 3; i++){

//indices of neighbor triangle
int v0 = i*2;
int nb = (i*2+1);
int v1 = (i*2+2) % 6;

bool deg = (gl_in[v0].gl_Position == gl_in[nb].gl_Position) ||( gl_in[v1].gl_Position == gl_in[nb].gl_Position)||( gl_in[v1].gl_Position == gl_in[v0].gl_Position);


vec3 facenormaladjacent[3];

//calculate face normals at each adjacent in eye coordinates
facenormaladjacent[0] = cross(
(modelview*vec4(gl_in[nb].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[v0].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[v1].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[v0].gl_Position.xyz,1.0)).xyz
);


facenormaladjacent[1] =  cross(
(modelview*vec4(gl_in[v1].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[nb].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[v0].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[nb].gl_Position.xyz, 1.0)).xyz
);

facenormaladjacent[2] =  cross(
(modelview*vec4(gl_in[v0].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[v1].gl_Position.xyz,1.0)).xyz, 
(modelview*vec4(gl_in[nb].gl_Position.xyz,1.0)).xyz - (modelview*vec4(gl_in[v1].gl_Position.xyz,1.0)).xyz
);


//test and see if the adjacent triangle changes direction (towards vs away) wrt the light

if( deg || faces_light != (dot(eyelight, facenormaladjacent[0]) > 0.0 
   || dot(eyelight, facenormaladjacent[1]) > 0.0 
   || dot(eyelight, facenormaladjacent[2]) > 0.0
))

{

//preserve the winding of the extruded edge
int i0 =   faces_light?v0:v1;
int i1 =   faces_light?v1:v0;

//generate the faces extruded to infinity

// translate to world space
vec4 lpos1 = gl_in[i0].gl_Position;
vec4 wtrans1 = modelmatrix * lpos1;

//we're going to make an extruded quad, so we need two vertices from the original triangle
vec4 lpos2 = gl_in[i1].gl_Position;
vec4 wtrans2 = modelmatrix * lpos2;

//multiplication of projection and camera matrix
gl_Position =cameramatrix * wtrans1;
EmitVertex();

//assume light is at 0,0,0 in wsp, so we just extrude the world space position to infinity
vec4 vert1inf = vec4( - lightdir, 0.0);
gl_Position = cameramatrix * vert1inf;
EmitVertex();

gl_Position = cameramatrix * wtrans2;
EmitVertex();

vec4 vert2inf = vec4( - lightdir, 0.0);
gl_Position = cameramatrix * vert2inf;
EmitVertex();

EndPrimitive();


} //the triangle is not facing the light! thus we have a silhouette edge to extrude





}


}

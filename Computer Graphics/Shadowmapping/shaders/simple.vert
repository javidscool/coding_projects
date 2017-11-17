varying vec3 normal;
uniform mat4 lightP;
uniform mat4 lightV;
uniform mat4 cameraV;
uniform mat4 cameraW;
uniform mat4 lightTigerMV;
uniform mat4 lightFloorMV;
uniform mat4 lightWallMV;
varying vec4 lightPos;
uniform int obj;

void main()
{
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	//gives you clip space from camera's perspective
	gl_Position = gl_ProjectionMatrix * cameraV * cameraW * gl_Vertex;
	
	if(obj>0)	// if its the tiger
		lightPos = lightP * lightTigerMV * gl_Vertex;
	else if(obj < 0)	//if its the wall
		lightPos = lightP * lightWallMV * gl_Vertex;
	else		// if its the floor
		lightPos = lightP * lightFloorMV * gl_Vertex;
		
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
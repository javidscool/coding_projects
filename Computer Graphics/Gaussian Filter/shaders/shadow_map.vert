varying vec3 normal;
uniform mat4 lightP;
uniform mat4 lightV;
uniform mat4 cameraV;
uniform mat4 cameraW;
varying vec4 lightPos;

void main()
{
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	//gives you clip space from camera's perspective
	gl_Position = gl_ProjectionMatrix * cameraV * cameraW * gl_Vertex;
	
	//lightP is light's projection matrix, lightV is light's view matrix and cameraW is camera's world matrix
	//together lightV and cameraW give you the modelView matrix for light's perspective
	//therefore lightPos will give you clip space from light's perspective
	lightPos = lightP * lightV * cameraW * gl_Vertex;
		
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
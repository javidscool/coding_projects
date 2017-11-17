#version 120

varying vec3 worldPos;
varying vec3 worldNormal;
varying vec2 textureCoords;

uniform sampler2D diffTexture;
uniform sampler2D robotTexture;

void main(){

	vec3 normalRay = normalize(worldNormal);


	float phi = acos(normalRay.y); 

	float theta;
	theta = atan(-normalRay.z, normalRay.x);//z was neg
	

	//atan returns values from neg pi to pi.  
	//acos returns values from 0 to pi

	//we now make these into uv's
	vec2 uvCoord = vec2( .5 + .5 * (theta / 3.14), ( 1.0 - (phi) / 3.14) );

	gl_FragColor = texture2D(diffTexture, uvCoord.st);
	//gl_FragColor = texture2D(robotTexture, textureCoords);
 	//* texture2D(robotTexture, textureCoords)
}

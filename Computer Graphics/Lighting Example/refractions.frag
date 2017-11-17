varying vec3 worldPos;
varying vec3 worldNormal;
varying vec3 worldCamera;

uniform sampler2D texture;

void main(){

	vec3 incidentRay = normalize(worldCamera - worldPos);
	vec3 normalRay = normalize(-worldNormal);

	float snellDivide = (1.0f/1.33f);
	float cI = dot(normalRay, -incidentRay);
	float v = 1 + ( snellDivide * snellDivide ) * ( (cI * cI) - 1);


	vec3 refractedRay = (snellDivide * incidentRay) + (snellDivide*cI - sqrt(v)) * (normalRay);

	//normalizing the reflection ray even though I don't need to
	vec3 normalizedRefractedRay = normalize(refractedRay);


	float phi = acos(normalizedRefractedRay.y); 

	float theta;
	theta = atan(-normalizedRefractedRay.z, normalizedRefractedRay.x);//z was neg
	

	//atan returns values from neg pi to pi.  
	//acos returns values from 0 to pi

	//we now make these into uv's
	vec2 uvCoord = vec2( .5 + .5 * (theta / 3.14), ( 1.0 - (phi) / 3.14) );

	gl_FragColor = texture2D(texture, uvCoord.st);
}

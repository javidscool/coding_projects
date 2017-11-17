varying vec3 worldPos;
varying vec3 worldNormal;
varying vec3 worldCamera;

uniform sampler2D texture;

void main(){
	vec3 incidentRay = normalize(worldCamera - worldPos);
	vec3 normalRay = normalize(worldNormal);
	vec3 reflectionRay = reflect(incidentRay, normalRay);

	//normalizing the reflection ray even though I don't need to
	vec3 normalizedRefRay = normalize(reflectionRay);


	float phi = acos(normalizedRefRay.y); 

	float theta;
	theta = atan(-normalizedRefRay.z, normalizedRefRay.x);//z was neg
	

	//atan returns values from neg pi to pi.  
	//acos returns values from 0 to pi

	//we now make these into uv's
	vec2 uvCoord = vec2( .5 + .5 * (theta / 3.14), ( 1.0 - (phi) / 3.14) );

	gl_FragColor = texture2D(texture, uvCoord.st);
}

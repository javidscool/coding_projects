varying vec3 direction;
uniform sampler2D texture;


void main(){

	vec3 lNormal = normalize(-direction);

	float phi = acos(lNormal.y); 

	float theta;
	theta = atan(-lNormal.z, lNormal.x);
	

	//atan returns values from neg pi to pi.  
	//acos returns values from 0 to pi

	//we now make these into uv's for a latitude longitude map
	vec2 uvCoord = vec2( .5 + .5*(theta / 3.14), ( 1.0 - (phi) / 3.14) );

	gl_FragColor =  texture2D(texture, uvCoord.st);
}

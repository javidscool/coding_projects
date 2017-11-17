varying vec3 normal;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3; //texture for brick wall
uniform sampler2D tex4; //texture for stained glass
uniform int obj;
varying vec4 lightPos;
uniform float texsize;


void main()
{	
	//tex1 stores tiger texture
	vec4 tex1_col = tex2D(tex1, gl_TexCoord[0].st);
	//tex3 stores brick wall texture
	vec4 tex3_col = tex2D(tex3, gl_TexCoord[0].st);
		
	//taking z value distance from point camera sees to light source image plane
	//looking from camera's POV
	float zComp = lightPos.z/ lightPos.w;
	
	//converting the S and T coordinates to the point the camera sees from the lights perspective on the shadow map
	//converting from clip space to texture space
	float textS = (lightPos.x / lightPos.w + 1.0) / 2.0;
	float textT = (lightPos.y / lightPos.w + 1.0) / 2.0; 
	
		
	float zBias = 0.005;
	float temp = 0.0;
	float sum = 0.0;
	float avg;
	
	//this makes your average equal to 0.0 (dark) if you're outside of the lights perspective (shadow can't be here)
	if(textS > 1.0 || textS < 0.0 || textT > 1.0 || textT < 0.0){
		avg = 0.0;
	}
	
	else{
		for(int s = -5; s < 5; s++){
			for(int t = -5; t < 5; t++){
				//Note: 2 different x and y coordinates on object have same texture coordinate on shadow map.  It's these points that you compare.
				//sampling the shadow map to see the which z-values are different for two points with the same x and y coordinates 
				//looking from shadow map's perspective
				temp = tex2D(tex2, vec2(textS,textT) + /*adding the offset*/ vec2(s,t)* texsize ).z;
				if(zComp - temp < zBias){
					sum += 1.0;
					}
				else{
					sum += 0.0;
					}
				}
			}
		//finding the percent in-shadow a point is
		avg = sum / 100.0;
	}
	
	
	//multiply the tiger and floor by the avg to get the percentage the shadow is giving to the fragment being evaluated
	if(obj>0)	// if its the tiger
		gl_FragColor = tex1_col * avg * tex2D(tex4, vec2(textS, textT) /*this samples the stained glass texture from the lights perspective, which in turn makes it spread out and distorted*/);
	else		// if its the floor or wall (cause they're the same texture)
		gl_FragColor = tex3_col * avg * tex2D(tex4, vec2(textS, textT) /*this samples the stained glass texture from the lights perspective, which in turn makes it spread out and distorted*/);
				 
}
varying vec3 normal;
uniform sampler2D tex1;
uniform sampler2D tex2;
varying vec4 lightPos;
uniform int obj;
uniform int pass;

void main()
{	
	//tex1 stores tiger texture
	vec4 tex1_col = tex2D(tex1, gl_TexCoord[0].st);
	//tex2 stores the shadow map
	vec4 tex2_col = tex2D(tex2, gl_TexCoord[0].st);

	
	//if we're on pass 1, draw the three shadow's to the three different render targets (this is done in main) 
	if(pass == 1){
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
	
		temp = tex2D(tex2, vec2(textS,textT)).x;
		if(zComp - temp < zBias){
			sum = 1.0;
		}
		
		//this makes your average equal to 1.0 (light) if you're outside of the lights perspective (shadow can't be here)
		else if(textS > 1.0 || textS < 0.0 || textT > 1.0 || textT < 0.0){
			sum = 1.0;
		}
		else{
			sum = 0.0;
		}
	
		gl_FragColor = vec4(1, 1, 1, 1) * sum;
	}
	
	//if we're on pass 2, just draw the tiger and the floor (again, code in main)
	else if(pass == 2){
		if(obj>0)	// if its the tiger
			gl_FragColor = tex1_col;
		else		// if its the floor or wall (cause they're the same color)
			gl_FragColor = vec4(1, 0, 0, 1); 
	}
				
}

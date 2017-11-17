varying vec4 posi;
uniform float texsize;
uniform sampler2D texture;
uniform int pass;

void main()
{	
	vec4 texture_col = {0,0,0,1};
	
	//converting from clip space to texture space for points on the quad
	float textS = (posi.x / posi.w + 1.0) / 2.0;
	float textT = (posi.y / posi.w + 1.0) / 2.0; 
	
	//setting one row of kernel values
	
	//gaussian kernel
	//float kernel[5] = float[5](.0096, .2054, .5699, .2054, .0096);
	
	//average kernel
	float kernel[5] = float[5](.2, .2, .2, .2, .2);
	
	//initializing gl_FragColor to black
	gl_FragColor = vec4(0, 0, 0, 1);
	
	int sample_width = 5;
	int i;
	
	//if we're on pass 1, we're going to blur the shadow we pass in around the x-axis ONLY
	if(pass == 1){
	
		for(i = -(sample_width / 2); i <= (sample_width / 2); i++){
			//if texture coordinate that we're sampling and moving through with the loop is less than 1 or greater than 0, use it
			//else discard it
			if((textS + i*texsize) < 1.0 && (textS + i*texsize) > 0.0){
				//going thru the x-axis only
				texture_col = tex2D(texture, vec2(i*texsize + textS,textT));
				gl_FragColor += kernel[i+(sample_width/2)] * texture_col;
			}
		}
	}
	

	//if we're on pass 2, we're going to blur the shadow we pass in around the y-axis ONLY
	else if(pass == 2){
					
		for(i = -(sample_width / 2); i <= (sample_width / 2); i++){
			//if texture coordinate that we're sampling and moving through with the loop is less than 1 or greater than 0, use it
			//else discard it
			if((textT + i*texsize) < 1.0 && (textT + i*texsize) > 0.0){
				//going thru the y-axis only
				texture_col = tex2D(texture, vec2(textS,i*texsize + textT));
				gl_FragColor += kernel[i+(sample_width/2)] * texture_col;
			}
		}
	}	
		
}

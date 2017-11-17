varying vec4 posi;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;

void main()
{
	//converting from clip space to texture space for points on the quad
	float textS = (posi.x / posi.w + 1.0) / 2.0;
	float textT = (posi.y / posi.w + 1.0) / 2.0; 
	
	vec4 tex3_col = {0,0,0,1};
	vec4 tex4_col = {0,0,0,1};
	vec4 tex5_col = {0,0,0,1};
	vec4 tex6_col = {0,0,0,1};
	
	tex3_col = tex2D(tex3, vec2(textS,textT));
	tex4_col = tex2D(tex4, vec2(textS,textT));
	tex5_col = tex2D(tex5, vec2(textS,textT));
	tex6_col = tex2D(tex6, vec2(textS,textT));
	
	gl_FragColor = tex3_col * tex4_col * tex5_col * tex6_col;
}
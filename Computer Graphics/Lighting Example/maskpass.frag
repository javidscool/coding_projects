uniform sampler2D depthtexture;

varying vec4 lightspacepos;

//varying vec4 clipPos;

void main(){

  
  
float z = abs(lightspacepos.z / lightspacepos.w);

  vec2 texc = ((lightspacepos.xy / lightspacepos.w) + 1.0) / 2.0;

  float lightz = abs(texture2D(depthtexture, texc).r);

  if(z > lightz + .001){
	gl_FragColor = vec4(vec3(0.0), 1.0);
  }else{
	gl_FragColor = vec4(vec3(1.0), 1.0);
	}

//gl_FragColor = vec4(texc, 0.0, 1.0);

}

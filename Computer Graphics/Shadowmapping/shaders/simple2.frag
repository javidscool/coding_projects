varying vec4 posi;

void main()
{
	//you're setting the frag color for the shadow map and you do this 
	//by getting the depth (posi.z) for the objects closest to your light source
	//from the light's view
	gl_FragColor = vec4(0, 0, posi.z/posi.w, 1);
}
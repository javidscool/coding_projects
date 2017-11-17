varying vec4 eyeposition;

void main(){

	gl_FragColor = vec4((eyeposition.z / eyeposition.w), 0.0,0.0,0.0);

}

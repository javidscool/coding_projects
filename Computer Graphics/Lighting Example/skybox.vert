attribute vec3 position;

varying vec3 direction;

uniform mat4 projection;

void main(){
	direction = normalize(position);

	projection[3] = vec4(0,0,0,1);

	gl_Position = projection * vec4(position, 1.0);

}

varying vec4 posi;

void main()
{
	//going from object space to clip space
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	posi = gl_Position;
}
varying vec4 posi;

void main()
{
	posi = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = posi;
}
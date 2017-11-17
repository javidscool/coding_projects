varying vec4 posi;

void main()
{
	gl_Position = gl_Vertex;
	//clip space position of points on quad (space to work on)
	posi = gl_Position;
}
in vec4 position;
out vec4 color;

void main()
{
	color = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	ftransform();
}

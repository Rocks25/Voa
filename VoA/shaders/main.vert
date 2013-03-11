uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

in vec4 position;
out vec4 color;

void main()
{
	color = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = projMat * viewMat * modelMat * gl_Vertex;
	ftransform();
}

#include "../include/GLShaderProgram.h"

GLShaderProgram::GLShaderProgram(char *name)
{
	_Name = name;
}

GLShaderProgram::~GLShaderProgram()
{
	for(unsigned int i=0; i<_Shaders.size(); i++)
	{
		_Shaders.pop();
	}
}

void GLShaderProgram::Create()
{
	programID = glCreateProgram();
}

GLuint GLShaderProgram::GetProgramID()
{
	return programID;
}

char *GLShaderProgram::GetName()
{
	return _Name;
}

void GLShaderProgram::NewVertexShader(char *filename)
{
	GLShader *vshader = new GLShader(GL_VERTEX_SHADER, filename);
	vshader->Init();
	glAttachShader(programID,vshader->GetShaderID());
	_Shaders.push(vshader);
}

void GLShaderProgram::NewFragmentShader(char *filename)
{
	GLShader *fshader = new GLShader(GL_FRAGMENT_SHADER, filename);
	fshader->Init();
	glAttachShader(programID,fshader->GetShaderID());
	_Shaders.push(fshader);
}

void GLShaderProgram::Link()
{
	glLinkProgram(programID);
}

void GLShaderProgram::Bind()
{
	glUseProgram(programID);
}

void GLShaderProgram::SetUniformValue(const char *name, int value)
{
	GLint pos = glGetUniformLocation(programID,name);
	glUniform1i(pos,value);
}
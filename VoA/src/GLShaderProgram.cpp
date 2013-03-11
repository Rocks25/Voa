#include "../include/GLShaderProgram.h"

GLShaderProgram::GLShaderProgram()
{
}

GLShaderProgram::~GLShaderProgram()
{
    //dtor
}

void GLShaderProgram::Create()
{
	programID = glCreateProgram();
}

GLuint GLShaderProgram::GetProgramID()
{
	return programID;
}

std::vector<GLShader *> GLShaderProgram::GetShaders()
{
	return shaders;
}

void GLShaderProgram::AttachShader(GLShader *shader)
{
	glAttachShader(programID,shader->GetShaderID());
	shaders.push_back(shader);
}

void GLShaderProgram::Link()
{
	glLinkProgram(programID);
}

void GLShaderProgram::Bind()
{
	glUseProgram(programID);
}

void GLShaderProgram::UnBind()
{
	glUseProgram(0);
}

void GLShaderProgram::SetUniformValue(const char *name, int value)
{
	GLint pos = glGetUniformLocation(programID,name);
	glUniform1i(pos,value);
}
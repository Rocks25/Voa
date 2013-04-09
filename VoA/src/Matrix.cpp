#include "..\include\Matrix.h"
#include "..\include\WindowManager.h"


Matrix::Matrix(void)
{
}


Matrix::~Matrix(void)
{
}

void Matrix::Translate(glm::vec3 trans)
{
	_CurrMatrix = glm::translate(_CurrMatrix, trans);
}

void Matrix::Translate(float x, float y, float z)
{
	_CurrMatrix = glm::translate(_CurrMatrix, glm::vec3(x,y,z));
}

void Matrix::Rotate(float angle, glm::vec3 rot)
{
	_CurrMatrix = glm::rotate(_CurrMatrix, angle, rot);
}

void Matrix::Rotate(float angle, float x, float y, float z)
{
	_CurrMatrix = glm::rotate(_CurrMatrix, angle, glm::vec3(x,y,z));
}

void Matrix::Scale(glm::vec3 scale)
{
	_CurrMatrix = glm::scale(_CurrMatrix,scale);
}

void Matrix::Scale(float x, float y, float z)
{
	_CurrMatrix = glm::scale(_CurrMatrix, x, y, z);
}

glm::mat4 *Matrix::GetCurrentMatrix()
{
	return &_CurrMatrix;
}

glm::mat4 Matrix::PopMatrix()
{
	glm::mat4 mat = _CurrMatrix;
	_CurrMatrix = _Stack[_Stack.size()-1];
	_Stack.pop_back();
	return mat;
}

void Matrix::PushMatrix()
{
	_Stack.push_back(_CurrMatrix);
	_CurrMatrix = glm::mat4();
}

void Matrix::LoadIdentity()
{
	_CurrMatrix = glm::mat4();
}

float *Matrix::data()
{
	return &_CurrMatrix[0][0];
}


void Matrix::SetupOrthProject()
{
	_CurrMatrix = glm::ortho(0.0f, (float)WM->GetWindowWidth(), (float)WM->GetWindowHeight(), 0.0f, -50.0f, 100.0f);
}

void Matrix::SetupPerspProject()
{
	_CurrMatrix = glm::perspective(45.0f,(float)WM->GetWindowWidth()/((float)WM->GetWindowHeight()),0.1f,1000.0f);
}


void Matrix::UpdateShader(const char *name)
{
	int projMatLoc = glGetUniformLocation(WM->GetRenderer()->GetCurrentShader()->GetProgramID(),name);	// Get the location of the projection matrix from shader

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, data());					// Send the perspective matrix to the shader program
}
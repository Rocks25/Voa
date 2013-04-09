#include "..\include\Camera.h"
#include "..\include\WindowManager.h"


Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::Translate(glm::vec3 trans)
{
	_CurrMatrix = glm::translate(_CurrMatrix, trans);
}

void Camera::Translate(float x, float y, float z)
{
	_CurrMatrix = glm::translate(_CurrMatrix, glm::vec3(x,y,z));
}

void Camera::Rotate(float angle, glm::vec3 rot)
{
	_CurrMatrix = glm::rotate(_CurrMatrix, angle, rot);
}

void Camera::Rotate(float angle, float x, float y, float z)
{
	_CurrMatrix = glm::rotate(_CurrMatrix, angle, glm::vec3(x,y,z));
}

glm::mat4 *Camera::GetCurrentMatrix()
{
	return &_CurrMatrix;
}

glm::mat4 Camera::PopMatrix()
{
	glm::mat4 mat = _CurrMatrix;
	_CurrMatrix = _Stack[_Stack.size()-1];
	_Stack.pop_back();
	return mat;
}

void Camera::PushMatrix()
{
	_Stack.push_back(_CurrMatrix);
	_CurrMatrix = glm::mat4();
}

float *Camera::data()
{
	return &_CurrMatrix[0][0];
}

void Camera::UpdateShader()
{
	int projMatLoc = glGetUniformLocation(WM->GetRenderer()->GetCurrentShader()->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, data());					// Send the perspective matrix to the shader program
}
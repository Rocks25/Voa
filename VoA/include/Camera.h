#pragma once

#include <glm\gtc\matrix_transform.hpp>
#include <vector>

class Camera
{
protected:
	glm::mat4 _CurrMatrix;
	std::vector<glm::mat4> _Stack;
public:
	Camera(void);
	~Camera(void);
	void Translate(glm::vec3 trans);
	void Translate(float x, float y, float z);
	void Rotate(float angle, glm::vec3 rot);
	void Rotate(float angle, float x, float y, float z);
	glm::mat4 *GetCurrentMatrix();
	glm::mat4 PopMatrix();
	void PushMatrix();
	float *data();
	void UpdateShader();
};
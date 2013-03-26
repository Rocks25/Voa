#pragma once

#include "BasicObject.h"
#include "WindowFunctions.h"
#include "GLShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class About
{
public:
	About();
	~About(void);
	void Render();
	void Init();
	void Cleanup();
	void Resize();
private:
	void InitTextures();
	bool IsInitialized();

	std::vector<BasicObject *> objects;
	float alpha1,alpha2,arate1,arate2;
	bool a1inc,a2inc;
	float a1rot,a2rot;
	glm::mat4 pos1;
	glm::mat4 pos2;
	glm::mat4 pos3;
	glm::vec3 dir1;
	glm::vec3 dir2;
	glm::vec2 ScreenCenter;
	bool _Initialized;
};


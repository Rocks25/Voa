#pragma once

#include "BasicObject.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "vector.h"
#include "../include/Plane.h"

class OptionsMenu : public Scene
{
public:
	OptionsMenu(char *name);
	~OptionsMenu(void);
	virtual void Render();
	virtual void Init();
	void Cleanup();
	void Resize();
	void SelectNext();
	void SelectPrev();
	void SelectNextMode();
	void SelectPrevMode();
	int GetSelection();
	int GetModeIndex();

private:
	void InitTextures();
	bool IsInitialized();

	std::vector<Plane *> objects;
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
	int selection;
	int modeselected;
};


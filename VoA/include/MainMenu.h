#pragma once

#include "BasicObject.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "vector.h"
#include "../include/Plane.h"

class MainMenu : public Scene
{
public:
	MainMenu(char *name);
	~MainMenu(void);
	virtual void Render();
	void RenderMainMenu();
	void RenderOptionsMenu();
	virtual void Init();
	void Cleanup();
	void Resize();
	void SelectNext();
	void SelectPrev();
	void SelectNextMode();
	void SelectPrevMode();
	int GetSelection();
	int GetModeIndex();
	void SetSelection(int index);

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
	int selection;
	int modeselected;
	bool _Initialized;
};


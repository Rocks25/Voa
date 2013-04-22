#pragma once

#include "Scene.h"
#include <vector>

class SceneManager
{
private:
	std::vector<Scene *> _Scenes;
	Scene *_CurrScene;
public:
	SceneManager(void);
	~SceneManager(void);
	void Cleanup();
	void InitAll();
	void NewHUDElement(char *name);
	void AddElement(Scene *scene);
	bool BindScene(char *name);
	std::vector<Scene *> GetAllScenes();
	Scene *GetSceneByName(char *name);
	char *GetCurrentSceneName();
	Scene *GetCurrentScene();
	void RenderElement();
	void RenderElement(char *name);
	void RemoveElement(char *name);
};

extern SceneManager *SM;
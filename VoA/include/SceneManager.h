#pragma once

#include "Scene.h"
#include "vector.h"

class SceneManager
{
private:
	static SceneManager *_Instance;
	Vector<Scene *> _Scenes;
	Scene *_CurrScene;
protected:
	SceneManager(void);
	~SceneManager(void);
public:
	static SceneManager *GetSingleton();
	void NewScene(char *name);
	bool BindScene(char *name);
	Scene *GetSceneByName(char *name);
	char *GetCurrentSceneName();
	Scene *GetCurrentScene();
	void RenderScene();
	void RenderScene(char *);
};


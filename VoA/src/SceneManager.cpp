#include "..\include\SceneManager.h"

SceneManager *SceneManager::_Instance = 0;

SceneManager::SceneManager(void)
{
}


SceneManager::~SceneManager(void)
{
	delete _Instance;
}

SceneManager *SceneManager::GetSingleton()
{
	if(!_Instance)
		_Instance = new SceneManager();
	return _Instance;
}

void SceneManager::NewScene(char *name)
{
	Scene *newscene = new Scene(name);
	_Scenes.push(newscene);
	_CurrScene = _Scenes.Last();
}

bool SceneManager::BindScene(char *name)
{
	for(unsigned int i=0;i<_Scenes.size();i++)
	{
		if(_Scenes[i]->GetName() == name)
		{
			_CurrScene = _Scenes[i];
			return true;;
		}
	}
	return false;
}

Scene *SceneManager::GetSceneByName(char *name)
{
	for(unsigned int i=0;i<_Scenes.size();i++)
	{
		if(_Scenes[i]->GetName() == name)
		{
			return _Scenes[i];
		}
	}
	return _CurrScene;
}

char *SceneManager::GetCurrentSceneName()
{
	return _CurrScene->GetName();
}

Scene *SceneManager::GetCurrentScene()
{
	return _CurrScene;
}

void SceneManager::RenderScene()
{
	_CurrScene->Render();
}
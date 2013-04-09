#include "..\include\SceneManager.h"

SceneManager::SceneManager(void)
{
}


SceneManager::~SceneManager(void)
{
	Cleanup();
	
	for(unsigned int i=0; i<_Scenes.size(); i++)
	{
		delete _Scenes[i];
	}
}

void SceneManager::Cleanup()
{
	for(unsigned int i=0; i<_Scenes.size(); i++)
	{
		_Scenes[i]->Cleanup();
	}
}

void SceneManager::InitAll()
{
	for(unsigned int i=0; i<_Scenes.size(); i++)
	{
		_Scenes[i]->Init();
	}
}

void SceneManager::NewScene(char *name)
{
	Scene *newscene = new Scene(name);
	_Scenes.push_back(newscene);
	_CurrScene = _Scenes[_Scenes.size()-1];
}

void SceneManager::AddScene(Scene *scene)
{
	_Scenes.push_back(scene);
	_CurrScene = _Scenes[_Scenes.size()-1];
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

std::vector<Scene *> SceneManager::GetAllScenes()
{
	return _Scenes;
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

void SceneManager::RenderScene(char *name)
{
	Scene *scene = GetSceneByName(name);
	scene->Render();
}
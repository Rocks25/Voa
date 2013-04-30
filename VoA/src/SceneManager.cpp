#include "..\include\SceneManager.h"
#include "..\include\Error.h"

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

void SceneManager::NewHUDElement(char *name)
{
	Scene *newscene = new Scene(name);
	_Scenes.push_back(newscene);
	_CurrScene = _Scenes[_Scenes.size()-1];
}

void SceneManager::AddElement(Scene *scene)
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
			return true;
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
	char buf[255] = {0};
	sprintf(buf, "Could not find Scene '%s'!",name);
	//Error->NewError(buf);
	return 0;
}

char *SceneManager::GetCurrentSceneName()
{
	return _CurrScene->GetName();
}

Scene *SceneManager::GetCurrentScene()
{
	return _CurrScene;
}

void SceneManager::RenderElement()
{
	_CurrScene->Render();
}

void SceneManager::RenderElement(char *name)
{
	Scene *scene = GetSceneByName(name);
	scene->Render();
}


void SceneManager::RemoveElement(char *name)
{
	for(unsigned int i=0;i<_Scenes.size();i++)
	{
		if(_Scenes[i]->GetName() == name)
		{
			delete _Scenes[i];
			_Scenes.erase(_Scenes.begin()+i);
		}
	}
}
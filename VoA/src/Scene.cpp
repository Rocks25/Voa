#include "..\include\Scene.h"


Scene::Scene(char *name)
{
	_Name=name;
}


Scene::~Scene(void)
{
	Cleanup();
}

void Scene::Cleanup()
{
	if(!_entities.empty())
	{
		for(unsigned int i=0;i<_entities.size();i++)
		{
			_entities[i]->Cleanup();
			delete _entities[i];
		}
		_entities.clear();
	}
}

void Scene::Init()
{
	for(unsigned int i=0;i<_entities.size();i++)
	{
		_entities[i]->Init();
	}
}

void Scene::InitTextures()
{
	for(unsigned int i=0;i<_entities.size();i++)
	{
		_entities[i]->InitTextures();
	}
}

void Scene::AddEntity(Entity *ent)
{
	_entities.push_back(ent);
}

Entity *Scene::GetEntityByName(const char *name)
{
	Entity *ent=0;
	for(unsigned int i=0;i<_entities.size();i++)
	{
		if(_entities[i]->GetName() == name)
			ent = _entities[i];
	}
	return ent;
}

void Scene::Render()
{
	for(unsigned int i=0;i<_entities.size();i++)
	{
		_entities[i]->Render();
	}
}

char *Scene::GetName()
{
	return _Name;
}
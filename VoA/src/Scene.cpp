#include "..\include\Scene.h"
#include "..\include\Error.h"


Scene::Scene(char *name)
{
	_Name=name;
}


Scene::~Scene(void)
{
	Cleanup();
	if(!_Entities.empty())
	{
		_Entities.clear();
	}
}

void Scene::Cleanup()
{
	if(!_Entities.empty())
	{
		for(unsigned int i=0;i<_Entities.size();i++)
		{
			_Entities[i]->Cleanup();
			delete _Entities[i];
			_Entities.erase(_Entities.begin()+i);
		}
	}
}

void Scene::Init()
{
	for(unsigned int i=0;i<_Entities.size();i++)
	{
		_Entities[i]->Init();
	}
}

void Scene::InitTextures()
{
	for(unsigned int i=0;i<_Entities.size();i++)
	{
		_Entities[i]->InitTextures();
	}
}

void Scene::AddEntity(Entity *ent)
{
	_Entities.push_back(ent);
}

Entity *Scene::GetEntityByName(const char *name)
{
	Entity *ent = 0;
	for(unsigned int i=0;i<_Entities.size();i++)
	{
		if(_Entities[i]->GetName() == name)
			ent = _Entities[i];
	}
	if(ent==0)
	{
		char buf[255] = {0};
		sprintf(buf, "Could not find entity '%s'!",name);
		//Error->NewError(buf);
	}
	return ent;
}

std::vector<Entity*> Scene::GetAllEntities()
{
	return _Entities;
}

unsigned int Scene::GetNumEntities()
{
	return _Entities.size();
}

void Scene::Render()
{
	for(unsigned int i=0;i<_Entities.size();i++)
	{
		_Entities[i]->Render();
	}
}

char *Scene::GetName()
{
	return _Name;
}
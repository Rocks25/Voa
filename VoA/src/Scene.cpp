#include "..\include\Scene.h"
#include "..\include\SceneManager.h"
#include "..\include\Plane.h"
#include "..\include\Error.h"
#include "..\include\ship.h"
#include "..\include\Game.h"
#include "..\include\Asteroid.h"


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
		/*for(unsigned int i=0;i<_Entities.size();i++)
		{
			_Entities[i]->Cleanup();
			delete _Entities[i];
		}*/
		_Entities.clear();
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

void Scene::CheckCollisions()
{
	for(int i=0; i<_Entities.size(); i++)
	{
		Entity *e = _Entities[i];
		std::vector<Mesh *> m = e->GetAllMeshes();
		if(m.size()<=0)
			return;
		for(int j=0; j<m.size()-1; j++)
		{
			for(int k=j+1; k<m.size(); k++)
			{
				if(m[i]->GetName() != "Asteroid")
					continue;
				Asteroid *m1 = (Asteroid *)m[j];
				Asteroid *m2 = (Asteroid *)m[k];/*
				if(m1->GetCollisionDelay()>0)
				{
					m1->UpdateCollisionDelay();
					continue;
				}
				if(m2->GetCollisionDelay()>0)
				{
					m2->UpdateCollisionDelay();
					continue;
				}*/
				glm::vec3 m1pos = m1->GetPosition();
				float m1size = m1->GetCollisionRadius();
				glm::vec3 m2pos = m2->GetPosition();
				float m2size = m2->GetCollisionRadius();
				Ship *ship = (Ship *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("Player Ship");
				glm::vec3 shippos = ship->GetPosition();

				float shipsize = ship->GetCollisionRadius();
				float distancesq = sqrt(pow(m2pos.x-m1pos.x,2) + pow(m2pos.y-m1pos.y,2));
				float sizesq = m1size+m2size;

				if(distancesq < sizesq)
				{
					float m1angle = atan2(m2->GetVelocity().x-m1->GetVelocity().x, m2->GetVelocity().y-m1->GetVelocity().y)-3.14159/2;
					float m2angle = atan2(m1->GetVelocity().x-m2->GetVelocity().x, m1->GetVelocity().y-m2->GetVelocity().y)-3.14159/2;
					/*float m1angle = atan2(m1->GetVelocity().x,m1->GetVelocity().y)-3.14159/2;
					float m2angle = atan2(m1->GetVelocity().x,m1->GetVelocity().y)-3.14159/2;*/
					glm::vec3 m1vel = glm::vec3(sin(m1angle),cos(m1angle),0);
					glm::vec3 m2vel = glm::vec3(sin(m2angle),cos(m2angle),0);
					m1->SetVelocity(m1vel);
					m2->SetVelocity(m2vel);/*
					m1->ResetCollisionDelay();
					m2->ResetCollisionDelay();*/
				}
				//distancesq = (m1pos.x*m1pos.x + m1pos.y*m1pos.y) - (shippos.x*shippos.x + shippos.y*shippos.y);
				//sizesq = m1size*m1size+shipsize*shipsize;
				//if(distancesq < sizesq)
				//{
				//	//Game->Pause();
				//}

				distancesq = sqrt(pow(m1pos.x-shippos.x,2) + pow(m1pos.y-shippos.y,2));
				sizesq = m1size+50.0f;

				if(distancesq < sizesq)
				{
					float m1angle = atan2(m2->GetVelocity().x-m1->GetVelocity().x, m2->GetVelocity().y-m1->GetVelocity().y)-3.14159/2;
					glm::vec3 m1vel = glm::vec3(sin(m1angle),cos(m1angle),0);
					m1->SetVelocity(m1vel);
					//ship->ResetPosition();
					Game->StartNewGame();
				}
			}
		}
	}
}
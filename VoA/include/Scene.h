#pragma once

#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
	Scene(char *name);
	~Scene(void);
	virtual void Cleanup();
	virtual void Init();
	virtual void InitTextures();
//	virtual void Translate(float x, float y, float z);
//	virtual void Translate(glm::vec3 pos);
//	virtual void Rotate(float angle, float x, float y, float z);
//	virtual void Rotate(float angle, glm::vec3 dir);
//	virtual void Scale(float x, float y, float z);
//	virtual void Scale(glm::vec3 scale);
	virtual void AddEntity(Entity *ent);
	virtual unsigned int GetNumEntities();
	virtual std::vector<Entity *> GetAllEntities();
	virtual Entity *GetEntityByName(const char *name);
	virtual void Render();
	virtual char *GetName();
	void CheckCollisions();
	
protected:
	char *_Name;
	glm::mat4 projMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat;
	std::vector<Entity *> _Entities;
};


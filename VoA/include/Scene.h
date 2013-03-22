#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GLShaderProgram.h"
#include "Entity.h"

class Scene
{
public:
	Scene(void);
	~Scene(void);
	virtual void Cleanup();
	virtual void Init();
	virtual void InitTextures();
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 pos);
	void Rotate(float angle, float x, float y, float z);
	void Rotate(float angle, glm::vec3 dir);
	void Scale(float x, float y, float z);
	void Scale(glm::vec3 scale);
	void AddEntity(Entity *ent);
	Entity *GetEntityByName(const char *name);
	void Render(GLShaderProgram *program);

private:
	glm::mat4 projMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat;
	std::vector<Entity *> _entities;
};


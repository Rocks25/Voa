#pragma once

#include "Mesh.h"
#include "GLShaderProgram.h"
#include "vector.h"
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	Entity(const char *name);
	virtual ~Entity(void);
	virtual void Cleanup();
	virtual void Init();
	virtual void InitTextures();
	virtual void AddMesh(Mesh *mesh);
	const char *GetName();
	unsigned int GetNumMeshes();
	std::vector<Mesh *> GetAllMeshes();
	void SetName(const char *name);
	Mesh *GetMeshByName(const char *name);
	virtual glm::vec3 GetPosition();
	virtual void SetPosition(int x, int y, int z);
	virtual void SetPosition(glm::vec3 pos);
	virtual void Translate(float x, float y, float z);
	virtual void Translate(glm::vec3 pos);
	virtual void Rotate(float x, float y, float z);
	virtual void Rotate(glm::vec3 rot);
	virtual void Scale(float x, float y, float z);
	virtual void Scale(glm::vec3 scale);
	virtual void Render();

protected:
	const char *_Name;
	std::vector<Mesh *> _Meshes;
	glm::vec3 _Position;
	glm::vec3 _Rotation;
	glm::vec3 _Scale;
};


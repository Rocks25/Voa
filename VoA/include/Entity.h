#pragma once

#include "Mesh.h"
#include "GLShaderProgram.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	Entity(const char *name);
	virtual ~Entity(void);
	virtual void Cleanup();
	virtual void Init();
	virtual void InitTextures();
	void AddMesh(Mesh *);
	const char *GetName();
	void SetName(const char *name);
	Mesh *GetMeshByName(const char *name);
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 pos);
	void Rotate(float angle, float x, float y, float z);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(float x, float y, float z);
	void Scale(glm::vec3 scale);
	void ResetMatrix();
	void Render();

private:
	const char *_Name;
	std::vector<Mesh *> _Meshes;
	glm::mat4 _modelMat;
};


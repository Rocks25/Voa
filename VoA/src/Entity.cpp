#include "..\include\Entity.h"
#include <glm/gtx/transform.hpp>


Entity::Entity(const char *name) : _modelMat(1.0f)
{
	_Name = name;
}


Entity::~Entity(void)
{
}

void Entity::Init()
{
	for(unsigned int i=0; i<_Meshes.size();i++)
	{
		_Meshes[i]->Init();
	}
}

void Entity::InitTextures()
{
	for(unsigned int i=0; i<_Meshes.size();i++)
	{
		_Meshes[i]->InitTextures();
	}
}

void Entity::AddMesh(Mesh *mesh)
{
	if(GetMeshByName(mesh->GetName())==0)
		_Meshes.push_back(mesh);
}

void Entity::SetName(const char *name)
{
	_Name = name;
}

const char *Entity::GetName()
{
	return _Name;
}

Mesh *Entity::GetMeshByName(const char *name)
{
	for(unsigned int i=0;i<_Meshes.size();i++)
	{
		if(_Meshes[i]->GetName() == name)
		{
			return _Meshes[i];
		}
	}
	return 0;
}

void Entity::Cleanup()
{
}

void Entity::Translate(float x, float y, float z)
{
	_modelMat = glm::translate(_modelMat,x,y,z);
}

void Entity::Translate(glm::vec3 pos)
{
	_modelMat = glm::translate(_modelMat,pos);
}

void Entity::Rotate(float angle, float x, float y, float z)
{
	_modelMat = glm::rotate(_modelMat,angle, x,y,z);
}

void Entity::Rotate(float angle, glm::vec3 axis)
{
	_modelMat = glm::rotate(_modelMat,angle,axis);
}

void Entity::Scale(float x, float y, float z)
{
	_modelMat = glm::scale(_modelMat, x,y,z);
}

void Entity::Scale(glm::vec3 scale)
{
	_modelMat = glm::scale(_modelMat, scale);
}

void Entity::ResetMatrix()
{
	_modelMat = glm::mat4(1.0f);
}

void Entity::Render(GLShaderProgram *program)
{
	for(unsigned int i=0;i<_Meshes.size();i++)
	{
		_Meshes[i]->Render(program);
	}
}
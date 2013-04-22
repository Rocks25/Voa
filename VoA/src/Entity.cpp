#include "..\include\Entity.h"
#include "..\include\Error.h"
#include "..\include\GlobalMatrices.h"
#include <glm/gtx/transform.hpp>


Entity::Entity(const char *name)
{
	_Name = name;
}

Entity::~Entity(void)
{
	for(unsigned int i=0; i<_Meshes.size(); i++)
	{
		delete _Meshes[i];
	}
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
	else
		delete mesh;
}

void Entity::SetName(const char *name)
{
	_Name = name;
}

const char *Entity::GetName()
{
	return _Name;
}

unsigned int Entity::GetNumMeshes()
{
	return _Meshes.size();
}

std::vector<Mesh *> Entity::GetAllMeshes()
{
	return _Meshes;
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
	char buf[255] = {0};
	sprintf(buf, "Could not find mesh '%s'!",name);
	Error->NewError(buf);
	return 0;
}

void Entity::Cleanup()
{
	if(!_Meshes.empty())
	{
		_Meshes.clear();
	}
}

glm::vec3 Entity::GetPosition()
{
	return _Position;
}

void Entity::Translate(float x, float y, float z)
{
	_Position += glm::vec3(x,y,z);
}

void Entity::Translate(glm::vec3 pos)
{
	_Position += pos;
}

void Entity::Rotate(float x, float y, float z)
{
	_Rotation += glm::vec3(x,y,z);
}

void Entity::Rotate(glm::vec3 rot)
{
	_Rotation += rot;
}

void Entity::Scale(float x, float y, float z)
{
	_Scale *= glm::vec3(x,y,z);
}

void Entity::Scale(glm::vec3 scale)
{
	_Scale *= scale;
}

void Entity::Render()
{
	GMat->ModelMatrix()->PushMatrix();
	for(unsigned int i=0;i<_Meshes.size();i++)
	{
		GMat->ModelMatrix()->LoadIdentity();
		GMat->ModelMatrix()->Translate(_Meshes[i]->GetPosition());
		GMat->ModelMatrix()->Rotate(_Meshes[i]->GetRotation().x,glm::vec3(1,0,0));
		GMat->ModelMatrix()->Rotate(_Meshes[i]->GetRotation().y,glm::vec3(0,1,0));
		GMat->ModelMatrix()->Rotate(_Meshes[i]->GetRotation().z,glm::vec3(0,0,1));
		GMat->UpdateShader();
		_Meshes[i]->Render();
	}
	GMat->ModelMatrix()->PopMatrix();
}

void Entity::SetPosition(int x, int y, int z)
{
	_Position = glm::vec3(x,y,z);
}

void Entity::SetPosition(glm::vec3 pos)
{
	_Position = pos;
}
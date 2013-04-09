#include "..\include\Mesh.h"


Mesh::Mesh(const char *name)
{
	_Name = name;
}


Mesh::~Mesh(void)
{
}

void Mesh::Cleanup()
{
}

void Mesh::Render()
{
}

void Mesh::SetName(const char *name)
{
	_Name=name;
}

const char *Mesh::GetName()
{
	return _Name;
}

void Mesh::Init()
{
}

void Mesh::InitTextures()
{
}

void Mesh::InitGeometry()
{
}

void Mesh::SetDirection(glm::vec3 dir)
{
    //_Direction=dir;
}

void Mesh::SetPosition(glm::vec3 pos)
{
	_Position = pos;
}

void Mesh::SetRotation(glm::vec3 rot)
{
	_Rotation += rot;
	rot = sqrt(rot.x*rot.x+rot.y*rot.y)/rot;
	SetDirection(rot);
	/*pEngineL.SetPositionMat(tmpPos*pEngineL.GetPositionMat());
	pEngineR.SetPositionMat(tmpPos*pEngineR.GetPositionMat());*/
}

void Mesh::SetScale(glm::vec3 scale)
{
	_Scale = scale;
}

void Mesh::Translate(glm::vec3 pos)
{
	_Position += pos;
}

void Mesh::Rotate(glm::vec3 rot)
{
	_Rotation += rot;
}

void Mesh::Scale(glm::vec3 scale)
{
	_Scale *= scale;
}


glm::vec3 Mesh::GetDirection()
{
	return _Direction;
}

glm::vec3 Mesh::GetPosition()
{
	return _Position;
}

glm::vec3 Mesh::GetRotation()
{
	return _Rotation;
}
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
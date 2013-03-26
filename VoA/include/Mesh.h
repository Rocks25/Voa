#pragma once

#include "WindowFunctions.h"
#include "GLShaderProgram.h"

class Mesh
{
public:
	Mesh(const char *name);
	virtual ~Mesh(void);
	const char *GetName();
	void SetName(const char *name);
	virtual void Render();
	virtual void Init();
	virtual void InitTextures();
	virtual void InitGeometry();
	virtual void Cleanup();

protected:
	const char *_Name;
	GLfloat *_Vertices;
	GLint *_TexCoords;
	GLubyte *_Indeces;
};


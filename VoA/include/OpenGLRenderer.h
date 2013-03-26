#pragma once

#include "Renderer.h"
#include "GLShaderProgram.h"
#include "vector.h"

class OpenGLRenderer : public Renderer
{
protected:
	Vector<GLShaderProgram *> _sPrograms;
	GLShaderProgram *_CurrShader;

public:
	OpenGLRenderer();
	~OpenGLRenderer(void);
	void Render();
	void InitFramebuffer();
	void InitShaders();
	bool NewShaderProgram(char *name);
	bool BindShaderProgram(char *name);
	GLShaderProgram *GetShaderProgramByName(char *name);
	GLShaderProgram *GetCurrentShader();
	void UnBindShaders();
};


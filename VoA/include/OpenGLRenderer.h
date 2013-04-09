#pragma once

#include "Renderer.h"
#include "GLShaderProgram.h"
#include "vector.h"
#include <glm/gtx/transform.hpp>

class OpenGLRenderer : public Renderer
{
protected:
	std::vector<GLShaderProgram> _sPrograms;
	GLShaderProgram *_CurrShader;
	unsigned int fbo_texture, fbo;
	//glm::mat4 orthMat,perspMat,viewMat;

public:
	OpenGLRenderer();
	~OpenGLRenderer(void);
	void Cleanup();
	void Render();
	void Init();
	void InitFramebuffer();
	void InitShaders();
	bool NewShaderProgram(char *name);
	bool BindShaderProgram(char *name);
	void BindFramebuffer();
	void UnbindFramebuffer();
	void BindFramebufferTexture();
	GLShaderProgram *GetShaderProgramByName(char *name);
	GLShaderProgram *GetCurrentShader();
	void UnBindShaders();
	void ReinitializeAll();
	void To3D();
	void To2D();
};


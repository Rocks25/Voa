#include "..\include\OpenGLRenderer.h"


OpenGLRenderer::OpenGLRenderer()
{
	
}


OpenGLRenderer::~OpenGLRenderer(void)
{
}

void OpenGLRenderer::Render()
{
}

void OpenGLRenderer::InitFramebuffer()
{
}

void OpenGLRenderer::InitShaders()
{
}

bool OpenGLRenderer::NewShaderProgram(char *name)
{
	for(unsigned int i=0;i<_sPrograms.size();i++)
	{
		if(_sPrograms[i]->GetName()==name)
			return false;
	}
	GLShaderProgram *prog = new GLShaderProgram(name);
	_sPrograms.push(prog);
	_sPrograms.Last()->Create();
	return true;
}

bool OpenGLRenderer::BindShaderProgram(char *name)
{
	for(unsigned int i=0; i<_sPrograms.size(); i++)
	{
		if(_sPrograms[i]->GetName()==name)
		{
			_CurrShader = _sPrograms[i];
			_CurrShader->Bind();
		}
	}
	return false;
}

GLShaderProgram *OpenGLRenderer::GetShaderProgramByName(char *name)
{
	for(unsigned int i=0;i<_sPrograms.size();i++)
	{
		if(_sPrograms[i]->GetName()==name)
		{
			return _sPrograms[i];
		}
	}
	return 0;
}

GLShaderProgram *OpenGLRenderer::GetCurrentShader()
{
	return _CurrShader;
}

void OpenGLRenderer::UnBindShaders()
{
	glUseProgram(0);
}
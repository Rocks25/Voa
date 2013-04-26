#include "..\include\OpenGLRenderer.h"
#include "..\include\WindowManager.h"
#include "..\include\GlobalMatrices.h"


OpenGLRenderer::OpenGLRenderer()
{
	
}


OpenGLRenderer::~OpenGLRenderer(void)
{
	glDeleteTextures(1,&fbo_texture);
}

void OpenGLRenderer::Cleanup()
{
	for(unsigned int i=0; i<_sPrograms.size(); i++)
	{
		std::vector<GLShader> shaders = _sPrograms[i].GetShaders();
		for(unsigned int j=0; j<shaders.size(); j++)
		{
			glDeleteShader(shaders[j].GetShaderID());
		}
		glDeleteProgram(_sPrograms[i].GetProgramID());
	}
}

void OpenGLRenderer::Render()
{
}

void OpenGLRenderer::Init()
{
	//InitFramebuffer();
	InitShaders();
}

void OpenGLRenderer::InitFramebuffer()
{
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	//glGenRenderbuffers(1, &fbo_depth);							// Generate depth buffer for frame buffer object (FBO)
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth);		// Bind depth buffer

	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window->w, window->h);
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);

	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	
	glGenTextures(1, &fbo_texture);					// Generate texture for frame buffer object
	glBindTexture(GL_TEXTURE_2D, fbo_texture);		// Bind the frame buffer's texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);			// Generate a standard rgba texture
	IsGLErrors("Framebuffer Init");

	// Setup Texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);				// Unbind texture

	glGenFramebuffersEXT(1, &fbo);					// Generate FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);	// Bind FBO

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0);		// Attach texture to FBO
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);	// Attach depth buffer to FBO

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// Unbind frame buffer object

	// Check status of FBO
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		MessageBox(NULL, "Couldn't create FBO!", "Init Error", MB_OK);
		return;
	}
	//IsGLErrors("Framebuffer Init");
	glBindTexture(GL_TEXTURE_2D, 0);
}




/****************************************************
*	Name: ShaderInit()								*
*	Description: Initializes all fragment and		*
*			vertex shaders.							*
****************************************************/

void OpenGLRenderer::InitShaders()
{
	int width=WM->GetWindowWidth();
	int height=WM->GetWindowHeight();

	GMat->ProjectionMatrix()->SetupOrthProject();

	if(!NewShaderProgram("Main"))
		MessageBox(NULL, "ERROR creating new shader 'Main'","ERROR",MB_OK);
	GLShaderProgram *shader = GetShaderProgramByName("Main");
	shader->NewFragmentShader("shaders/main.frag");			// Attach the vertex shader to the main program
	shader->NewVertexShader("shaders/main.vert");			// Attache the fragment shader to the main program
	shader->Link();											// Link the shader program
	if(!BindShaderProgram("Main"))
		MessageBox(NULL, "Error Binding 'Main'","ERROR",MB_OK);
	IsGLErrors("Shader Init");

	GMat->UpdateShader();

	//Blur Shader Horizontal
	NewShaderProgram("HBlur");
	shader = GetShaderProgramByName("HBlur");
	shader->NewVertexShader("shaders/main.vert");		// Load the main vertex shader from file.
	shader->NewFragmentShader("shaders/blurH.frag");	// Load the main fragment shader from file.
	shader->Link();							// Link the shader program
	BindShaderProgram("HBlur");
	
	
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),0);

	GMat->UpdateShader();
	
	//Blur Shader Vertical
	NewShaderProgram("VBlur");
	shader = GetShaderProgramByName("VBlur");
	shader->NewVertexShader("shaders/main.vert");		// Load the main vertex shader from file.
	shader->NewFragmentShader("shaders/blurV.frag");	// Load the main fragment shader from file.
	shader->Link();										// Link the shader program
	BindShaderProgram("VBlur");
	
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),0);

	GMat->UpdateShader();
	
	NewShaderProgram("Text");
	shader = GetShaderProgramByName("Text");
	shader->NewFragmentShader("shaders/text.frag");			// Attach the vertex shader to the main program
	shader->NewVertexShader("shaders/main.vert");			// Attache the fragment shader to the main program
	shader->Link();											// Link the shader program
	BindShaderProgram("Text");
	IsGLErrors("Shader Init");

	GMat->UpdateShader();

	IsGLErrors("Shader Init");
}

bool OpenGLRenderer::NewShaderProgram(char *name)
{
	for(unsigned int i=0;i<_sPrograms.size();i++)
	{
		if(_sPrograms[i].GetName()==name)
			return false;
	}
	GLShaderProgram prog(name);
	_sPrograms.push_back(prog);
	GetShaderProgramByName(name)->Create();
	return true;
}

bool OpenGLRenderer::BindShaderProgram(char *name)
{
	for(unsigned int i=0; i<_sPrograms.size(); i++)
	{
		if(_sPrograms[i].GetName()==name)
		{
			_CurrShader = &_sPrograms[i];
			_CurrShader->Bind();
			return true;
		}
	}
	return false;
}

GLShaderProgram *OpenGLRenderer::GetShaderProgramByName(char *name)
{
	for(unsigned int i=0;i<_sPrograms.size();i++)
	{
		if(_sPrograms[i].GetName()==name)
		{
			return &_sPrograms[i];
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

void OpenGLRenderer::BindFramebuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
}

void OpenGLRenderer::UnbindFramebuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void OpenGLRenderer::BindFramebufferTexture()
{
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	GetCurrentShader()->SetUniformValue("tex",0);
}

void OpenGLRenderer::ReinitializeAll()
{
	_sPrograms.clear();
	//glDeleteTextures(1, &fbo_texture);
	InitShaders();
	InitFramebuffer();
}

void OpenGLRenderer::To3D()
{
	WM->GetRenderer()->BindShaderProgram("Main");
	GMat->ProjectionMatrix()->SetupPerspProject();
	GMat->UpdateShader();
}

void OpenGLRenderer::To2D()
{
	WM->GetRenderer()->BindShaderProgram("Main");
	GMat->ProjectionMatrix()->SetupOrthProject();
	GMat->UpdateShader();
}
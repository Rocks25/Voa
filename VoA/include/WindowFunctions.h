#pragma once

#define GL_GLEXT_PROTOTYPES

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image.h>
#elif defined _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Windows.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include <GL/glew.h>

inline bool IsGLErrors(char *txt)
{
	int error = glGetError();
	char buf[255] = {0};

	if(error != GL_NO_ERROR)
	{
		if(error == GL_INVALID_VALUE)
			sprintf_s(buf, "%s: Invalid Value", txt);
		else if(error == GL_INVALID_ENUM)
			sprintf_s(buf, "%s: Invalid Enumerator", txt);
		else if(error == GL_INVALID_OPERATION)
			sprintf_s(buf, "%s: Invalid Operation", txt);
		else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
			sprintf_s(buf, "%s: Invalid Framebuffer Operation", txt);
		else if(error == GL_OUT_OF_MEMORY)
			sprintf_s(buf, "%s: Out Of Memory", txt);
		else if(error == GL_STACK_UNDERFLOW)
			sprintf_s(buf, "%s: Stack Underflow", txt);
		else if(error == GL_STACK_OVERFLOW)
			sprintf_s(buf, "%s: Stack Overflow", txt);
		else
		{
			sprintf_s(buf,"%s: Unknown OpenGL Error '%d' Occured!",txt, error);
		}
		MessageBox(NULL,buf,"OpenGL Error",MB_OK|MB_ICONERROR);
		return false;
	}
	return false;
}
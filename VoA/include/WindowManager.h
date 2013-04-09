#pragma once

#include "window_includes.h"
#include "sdl_invert.h"
#include "OpenGLRenderer.h"

class WindowManager
{
private:
	SDL_Surface *_Window;
	SDL_Rect **_FSModes;
	SDL_Rect *_CurrentFSMode;
	SDL_Rect _CurrentWMode;
	bool _Fullscreen;
	bool _GrabMouse;
	bool _GrabKeyboard;
	OpenGLRenderer *_Renderer;
	bool _Initialized;
public:
	WindowManager(void);
	~WindowManager(void);
	void Cleanup();
	void ProcessEvent(SDL_Event *event);
	bool CreateSDLWindow();
	bool InitOpenGL();
	int GetWindowWidth();
	int GetWindowHeight();
	SDL_Surface *GetWindowHandle();
	void ToggleFullscreen();
	void ShowNormal();
	void ShowFullscreen();
	bool IsFullscreen();
	void SetWindowedMode(int x, int y, int w, int h);
	void SetFullscreenMode(int w, int h);
	void SetWindowedMode(SDL_Rect Mode);
	void SetFullscreenMode(SDL_Rect *Mode);
	void Reinitialize();

	// Input Capture Methods
	void GrabMouse();
	void ReleaseMouse();
	bool IsMouseGrabbed();
	OpenGLRenderer *GetRenderer();
};

extern WindowManager *WM;

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
#pragma once
#include <SDL.h>
#include "../include/OpenGLRenderer.h"

class WindowManager
{
private:
	static WindowManager *_Instance;
	SDL_Surface *_Window;
	SDL_Rect **_FSModes;
	SDL_Rect *_CurrentFSMode;
	SDL_Rect *_CurrentWMode;
	bool _Fullscreen;
	bool _GrabMouse;
	bool _GrabKeyboard;
	OpenGLRenderer *_Renderer;
protected:
	WindowManager(void);
	~WindowManager(void);
public:
	void ProcessEvent(SDL_Event *event);
	static WindowManager *GetSingleton();
	bool CreateSDLWindow();
	bool InitOpenGL();
	int GetWindowWidth();
	int GetWindowHeight();
	void ToggleFullscreen();
	void ShowNormal();
	void ShowFullscreen();
	bool IsFullscreen();
	void SetWindowedMode(int x, int y, int w, int h);
	void SetFullscreenMode(int w, int h);
	void SetWindowedMode(SDL_Rect *Mode);
	void SetFullscreenMode(SDL_Rect *Mode);

	// Input Capture Methods
	void GrabMouse();
	void ReleaseMouse();
	bool IsMouseGrabbed();
	OpenGLRenderer *GetRenderer();
};


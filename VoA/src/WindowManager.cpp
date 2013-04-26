#include "..\include\WindowManager.h"
#include "..\include\window_includes.h"
#include "..\include\Game.h"
#include "..\include\TextureManager.h"
#include "..\include\MainMenu.h"
#include "..\include\OptionsMenu.h"

WindowManager::WindowManager(void)
{
	_Fullscreen = false;
	_GrabMouse=false;
	_Renderer = new OpenGLRenderer();
	_Initialized = false;
	_Running = true;
}


WindowManager::~WindowManager(void)
{
	delete _Renderer;
}

void WindowManager::Cleanup()
{
	GetRenderer()->Cleanup();
	TM->Cleanup();
	IsGLErrors("Cleanup()");
}

void WindowManager::ProcessEvent(SDL_Event *event)
{
	MainMenu *mainmenu = (MainMenu *)SM->GetSceneByName("Main Menu");
	OptionsMenu *optionsmenu = (OptionsMenu *)SM->GetSceneByName("Options Menu");
	switch(event->type)					// See which event it is
    {
		// When window is resized or is toggled between window and fullscreen
	case SDL_VIDEORESIZE:
		//Cleanup();
		InitOpenGL();
		IsGLErrors("Video Resize");
		if(!WM->IsFullscreen())
		{
			SetWindowedMode(0, 0, event->resize.w, event->resize.h);
		}
		glViewport(0,0,event->resize.w,event->resize.h);		// Resize OpenGL Viewport to fit the screen/window
		SM->InitAll();
		break;
		// When user quits the game
	case SDL_VIDEOEXPOSE:
		if(Game->GetCurrentMode() != GM_PLAY)
		{
			mainmenu->Resize();
			optionsmenu->Resize();
		}
		break;
    case SDL_QUIT:
		_Running=false;
        break;
	case SDL_ACTIVEEVENT:
		if(event->active.state & SDL_APPACTIVE)
		{
			if(event->active.gain)
				_Active = true;
			else
				_Active = false;
		}
		break;
	default:
		break;
	}
}

bool WindowManager::CreateSDLWindow()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {						// Initialize everything
        return false;											// or return that there was an error
    }

	TTF_Init();

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		return false;
	}

	_FSModes = SDL_ListModes(NULL, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN);
	if(_FSModes == (SDL_Rect**)0 || _FSModes == (SDL_Rect **)-1)
		return false;
	_CurrentFSMode=_FSModes[0];					// Point the global window variable to the new SDL surface
	
	if(_Fullscreen)
	{
		_Window=SDL_SetVideoMode(_CurrentFSMode->w, _CurrentFSMode->h, 0, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);			//			and resizable, or return that there has been an error
	}
	else
	{
		SDL_Rect Mode;
		Mode.x = Mode.y = 0;
		Mode.w = 800;
		Mode.h = 600;
		_CurrentWMode = Mode;
		_Window=SDL_SetVideoMode(_CurrentWMode.w, _CurrentWMode.h, 0, SDL_HWSURFACE | SDL_OPENGL);
	}

	SDL_WM_SetCaption( "VoA: Variations on Asteroids", NULL );

	GrabMouse();

    return true;	
}

bool WindowManager::InitOpenGL()
{
	//glShadeModel(GL_SMOOTH);									// Set the rendering of models, if we have any, to be smooth
    glClearColor(0.0f,0.0f,0.0f,0.0f);							// Set the clear color to be black
    glClearDepth(1.0f);											// Setup the Depth Buffer
	//glEnable(GL_CULL_FACE);									// Enable culling of faces
	//glCullFace(GL_BACK);										// Make sure not to draw the back of faces by default
	
    glDisable(GL_DEPTH_TEST);									// Because we are blending, we don't want it to originally check for depth
    glEnable(GL_BLEND);											// Enable Blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Perform
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);			// Nicest Perspective Calculations
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);						// Nicest Point Smoothing
	if(!_Initialized)
	{
		if(glewInit()!=GLEW_OK)
			return false;											// Initialize GL Extensions
	}

	IsGLErrors("GL Init");										// If there are any errors, tell us
	_Initialized = true;
	return true;
}

int WindowManager::GetWindowWidth()
{
	return _Window->w;
}

int WindowManager::GetWindowHeight()
{
	return _Window->h;
}

SDL_Surface *WindowManager::GetWindowHandle()
{
	return _Window;
}

void WindowManager::ToggleFullscreen()
{
	if(_Fullscreen)		// Check if we are already in fullscreen, and if we are
	{
		ShowNormal();		// Switch to windowed
	}
	else				// Otherwise
	{
		ShowFullscreen();		// Switch to Fullscreen
	}

	IsGLErrors("ToggleFullscreen");
}

void WindowManager::ShowNormal()
{
	if(_Fullscreen)
	{
		Cleanup();
		_Active=false;
		_Window=SDL_SetVideoMode(_CurrentWMode.w, _CurrentWMode.h, 0, SDL_HWSURFACE | SDL_OPENGL);		// Switch to Fullscreen
		Reinitialize();
	}
	_Fullscreen=false;
}

void WindowManager::ShowFullscreen()
{
	if(!_Fullscreen)
	{
		Cleanup();
		_Window=SDL_SetVideoMode(_CurrentFSMode->w, _CurrentFSMode->h, 0, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen
		Reinitialize();
	}
	_Fullscreen=true;
}

bool WindowManager::IsFullscreen()
{
	return _Fullscreen;
}

void WindowManager::SetWindowedMode(int x, int y, int w, int h)
{
	SDL_Rect Mode;
	Mode.x=x;
	Mode.y=y;
	Mode.w=w;
	Mode.h=h;
	_CurrentWMode = Mode;
}

void WindowManager::SetFullscreenMode(int w, int h)
{
	Cleanup();
	_Window=SDL_SetVideoMode(w, h, 0, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen
	_Fullscreen=true;
	Reinitialize();
	
	return;
}

void WindowManager::SetWindowedMode(SDL_Rect Mode)
{
	_CurrentWMode=Mode;

	Reinitialize();
}

void WindowManager::SetFullscreenMode(SDL_Rect *Mode)
{	
	Cleanup();
	_Window=SDL_SetVideoMode(Mode->w, Mode->h, 0, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen
	_Fullscreen=true;
	Reinitialize();
}


/**** Input Capture Methods ****/
/* Mouse */
void WindowManager::GrabMouse()
{
	_GrabMouse=true;
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);
}
void WindowManager::ReleaseMouse()
{
	_GrabMouse=false;
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(SDL_ENABLE);
}

bool WindowManager::IsMouseGrabbed()
{
	return _GrabMouse;
}

OpenGLRenderer *WindowManager::GetRenderer()
{
	return _Renderer;
}

void WindowManager::Reinitialize()
{
	glViewport(0,0,_Window->w,_Window->h);		// Resize OpenGL Viewport to fit the screen/window
	IsGLErrors("ToggleFullscreen");
	InitOpenGL();
	TM->ReloadTextures();
	GetRenderer()->ReinitializeAll();
	_Active=true;
}

SDL_Rect **WindowManager::GetAvailableModes()
{
	return _FSModes;
}

int WindowManager::GetNumAvailableModes()
{
	int i=0;
	char buf[255] = {0};
	while(_FSModes[i])
	{
		i++;
	}
	return i;
}

bool WindowManager::IsActive()
{
	return _Active;
}

bool WindowManager::IsRunning()
{
	return _Running;
}

void WindowManager::Quit()
{
	_Running = false;
}
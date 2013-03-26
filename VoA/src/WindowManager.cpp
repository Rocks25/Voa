#include "..\include\WindowManager.h"
#include "..\include\WindowFunctions.h"
#include "..\include\Game.h"
#include "..\include\TextureManager.h"
WindowManager *WindowManager::_Instance = NULL;

WindowManager::WindowManager(void)
{
	_Fullscreen = false;
	_GrabMouse=false;
	_Renderer = new OpenGLRenderer();
}


WindowManager::~WindowManager(void)
{
	delete _Instance;
	delete _Renderer;
}

WindowManager *WindowManager::GetSingleton()
{
	if(!_Instance)
		_Instance = new WindowManager();
	return _Instance;
}

void WindowManager::ProcessEvent(SDL_Event *event)
{
	switch(event->type)					// See which event it is
    {
		// When window is resized or is toggled between window and fullscreen
	case SDL_VIDEORESIZE:
		//Cleanup();
		glViewport(0,0,event->resize.w,event->resize.h);		// Resize OpenGL Viewport to fit the screen/window
		WindowManager::GetSingleton()->InitOpenGL();
		WindowManager::GetSingleton()->GetRenderer()->InitShaders();
		WindowManager::GetSingleton()->GetRenderer()->InitFramebuffer();
		TextureManager::GetSingleton()->ReloadTextures();
		break;
		// When user quits the game
    case SDL_QUIT:
		Game::Quit();
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

	_FSModes = SDL_ListModes(NULL, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN);
	if(_FSModes == (SDL_Rect**)0 || _FSModes == (SDL_Rect **)-1)
		return false;
	char buf[1024] = {0};
	sprintf_s(buf,"%d x %d",_FSModes[0]->w,_FSModes[0]->h);
	for(int i=1; _FSModes[i]; i++)
	{
		char tmp[20];
		sprintf_s(tmp,"\n%d x %d",_FSModes[i]->w,_FSModes[i]->h);
		strcat_s(buf,tmp);
	}
	MessageBox(NULL,buf,"Supported Modes",MB_OK);
	_CurrentFSMode=_FSModes[0];
	SetFullscreenMode(_CurrentFSMode);			//			and resizable, or return that there has been an error
												// Point the global window variable to the new SDL surface

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
	if(glewInit()!=GLEW_OK)
		return false;											// Initialize GL Extensions

	IsGLErrors("GL Init");										// If there are any errors, tell us
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

void WindowManager::ToggleFullscreen()
{
	if(_Fullscreen)		// Check if we are already in fullscreen, and if we are
	{
		_Window=SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_RESIZABLE);		// Switch to windowed
	}
	else				// Otherwise
	{
		_Window=SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen

	}
	_Fullscreen=!_Fullscreen;		// toggle fullscreen variable
}

void WindowManager::ShowNormal()
{
	if(_Fullscreen)				// Otherwise
	{
		_Window=SDL_SetVideoMode(_CurrentWMode->w, _CurrentWMode->h, 32, SDL_OPENGL | SDL_RESIZABLE);		// Switch to Fullscreen
		_Fullscreen=false;
	}
}

void WindowManager::ShowFullscreen()
{
	if(!_Fullscreen)				// Otherwise
	{
		_Window=SDL_SetVideoMode(_CurrentFSMode->w, _CurrentFSMode->h, 32, SDL_HWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen
		_Fullscreen=true;
	}
}

bool WindowManager::IsFullscreen()
{
	return _Fullscreen;
}

void WindowManager::SetWindowedMode(int x, int y, int w, int h)
{
	SDL_Rect *Mode = new SDL_Rect();
	Mode->x=x;
	Mode->y=y;
	Mode->w=w;
	Mode->h=h;
	delete _CurrentWMode;
	_CurrentWMode=Mode;
}

void WindowManager::SetFullscreenMode(int w, int h)
{
	SDL_Rect Mode;
	Mode.x=0;
	Mode.y=0;
	Mode.w=w;
	Mode.h=h;
	
	for(int i=0;_FSModes[i];i++)
	{
		if(_FSModes[i]->w==Mode.w && _FSModes[i]->h==Mode.h)
		{
			if((_Window = SDL_SetVideoMode(_FSModes[i]->w, _FSModes[i]->h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN))==NULL)
			{
				_Window = SDL_SetVideoMode(_CurrentFSMode->w, _CurrentFSMode->h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN);
				return;
			}
			_CurrentFSMode = _FSModes[i];
			return;
		}
	}
}

void WindowManager::SetWindowedMode(SDL_Rect *Mode)
{
	_CurrentWMode=Mode;
}

void WindowManager::SetFullscreenMode(SDL_Rect *Mode)
{	
	for(int i=0;_FSModes[i];i++)
	{
		if(_FSModes[i]==Mode)
		{
			if((_Window = SDL_SetVideoMode(Mode->w, Mode->h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN))==NULL)
			{
				_Window = SDL_SetVideoMode(_CurrentFSMode->w, _CurrentFSMode->h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN);
				return;
			}
			_CurrentFSMode = _FSModes[i];
			return;
		}
	}
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
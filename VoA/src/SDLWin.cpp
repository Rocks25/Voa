#include "../include/SDLWin.h"
#include "../include/window_includes.h"
#include "../include/Plane.h"
#include "../include/Font.h"
#include <glm/gtx/transform.hpp>
#include "../include/Game.h"
#include "../include/OptionsMenu.h"
#include "../include/Error.h"
#include "../include/SoundManager.h"
#include "../include/AsteroidSystem.h"
/****************************************************
*	Name: SDLWin()									*
*	Description: Constructor.						*
****************************************************/

GameManager *Game;

SDLWin::SDLWin()
{
	fullscreen=true;						// Initially we are not in fullscreen mode
	Perspective=true;
	_blurXrate = _blurYrate = -rand()%1000/1000.0f*(1/128.0f);
	_blurX = 1/64.0f;
	_blurY = 1.0f/128.0f;
	_blurXinc = _blurYinc = false;
	Game = new GameManager();
}

/****************************************************
*	Name: ~SDLWin()									*
*	Description: Destructor.						*
****************************************************/

SDLWin::~SDLWin()
{
	Cleanup();
}

/****************************************************
*	Name: Render()									*
*	Description: Draws everything to the screen.	*
****************************************************/

void SDLWin::Render()
{
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();

	WM->GetRenderer()->BindShaderProgram("Main");								// Bind the main shader program
	glColor4f(1.0f,1.0f,1.0f,1.0f);												// Set default color to white
    glClear(GL_COLOR_BUFFER_BIT);												// Clear Color and Depth

	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	To2D();
	
	if(Game->GetCurrentMode()==GM_PLAY)
	{
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("Background");
		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("White");
		Plane::Render(0,0,width, height);

		SM->RenderElement("Main");												// Render Game
		
		GMat->ModelMatrix()->LoadIdentity();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("MousePointer");
	
		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("MousePointerAlpha");

		Plane::Render((float)PC->GetMouseX(),(float)PC->GetMouseY(),50.0f,50.0f);

	}
	
	if(Game->GetCurrentMode()==GM_MAINMENU || Game->GetCurrentMode() == GM_OPTIONSMENU)
	{
		glm::mat4 tmp = glm::mat4(1.0f);
		GMat->ModelMatrix()->UpdateShader("modelMat");
	
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("White");

		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("White");
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		/*WM->GetRenderer()->BindFramebuffer();
		GMat->ModelMatrix()->LoadIdentity();
		GMat->UpdateShader();
		glViewport(0, 0, width, height);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		SM->RenderElement("Main Menu");
		//WM->GetRenderer()->UnbindFramebuffer();

		GMat->ModelMatrix()->Translate(0,0,0);
		GMat->UpdateShader();
/*
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		char buf[512] = {0};
		Color col(255,255,255);
		sprintf_s(buf,"X: %d",_mouseX);
		Font::Render(buf,"arial rounded",col,24);
		GMat->ModelMatrix()->Translate(0.0f,20.0f,0.0f);
		GMat->ModelMatrix()->UpdateShader("modelMat");
		sprintf_s(buf, "Y: %d",_mouseY);
		Font::Render(buf,"arial rounded",col,24);*/

		/*glActiveTexture(GL_TEXTURE0);
		WM->GetRenderer()->BindFramebufferTexture();
		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("White");
		To2D();
		GMat->ViewMatrix()->LoadIdentity();
		GMat->ModelMatrix()->PushMatrix();
		GMat->UpdateShader();
		Plane::RenderInverted(0,0,(float)width,(float)height);*/

		//RenderPostProcessing();

		GMat->ModelMatrix()->PopMatrix();
		GMat->UpdateShader();

		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	glColor4f(0.0f,0.0f,0.0f,0.5f);
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("Background");
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	Plane::Render(0,0,250/800.0f*width,64/600.0f*height);
	
	WM->GetRenderer()->BindShaderProgram("Text");
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("White");
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	Font::Render("Now Playing:","Text/ThrowMyHandsUpintheAir",Color(255,255,255,255),24);
	GMat->ModelMatrix()->Translate(0,32,0);
	GMat->UpdateShader();
	Font::Render(Sounds->GetCurrSongName(),"Text/ThrowMyHandsUpintheAir",Color(255,255,255,255),24);

	if(Game->IsDebugMode())
	{
		glColor4f(1.0f,0.0f,0.0f,1.0f);
		GMat->ModelMatrix()->Translate(250.0f,0,0);
		GMat->UpdateShader();
		//Error->PrintErrors();
		GMat->ModelMatrix()->PopMatrix();
		glColor3f(1.0f,1.0f,1.0f);

		//ModeDisplay();
	}
	IsGLErrors("Render()");
	SDL_GL_SwapBuffers();		//Swap buffers
}

/****************************************************
*	Name: Run()										*
*	Description: This is the main loop where all	*
*			the interesting things happen			*
****************************************************/

int SDLWin::Run()
{
	WM->CreateSDLWindow();										// Initialize SDL
	WM->InitOpenGL();											// Initialize OpenGL
	Game->InitResources();
	Game->Pause();												// Switch to Menu Mode
	WM->GetRenderer()->Init();									// Initialize Renderer
	InitGeometry();												// Create Menus
	InitTextures();												// Initialize Menu Textures
	WM->GetRenderer()->To2D();									// Switch to 2D Rendering

    SDL_Event Event;											// Create an event variable for catching events that SDL sends
    while(WM->IsRunning())									// Main Loop starts here
    {
        if(SDL_PollEvent(&Event)) {
			GEM->ProcessEvent(&Event);				// Send Events to the 'Events' function for processing
        }
		if(WM->IsActive())
		{
			Loop();						// Game loop processing
			Render();					// Draw to the screen
			_CrtCheckMemory( );
		}
		SDL_Delay(TimeLeft());			// Limit the frame rate
    }

    Cleanup();							// Once everything is done, make sure to clean up all loose ends
    return 1;							// Return 1 so that we know everything went fine
}

/****************************************************
*	Name: Cleanup()									*
*	Description: Cleanup all variables that need	*
*			cleaning.								*
****************************************************/

void SDLWin::Cleanup()
{
	if(Game->GetCurrentMode()==GM_QUIT)
	{
		SDL_Quit();														// Close Out
	}
}

/****************************************************
*	Name: Loop()									*
*	Description: Do all calculations before the		*
*			Render function draws to the screen		*
****************************************************/

void SDLWin::Loop()
{
	PC->ProcessControls();
	if(Game->GetCurrentMode() == GM_PLAY)
	{
		AsteroidSystem *as = (AsteroidSystem *)SM->GetSceneByName("Main")->GetEntityByName("Asteroids");
		as->Update();
		as->Spawn();
		if(SM->GetSceneByName("Main"))
			SM->GetSceneByName("Main")->CheckCollisions();
	}
}

/****************************************************
*	Name: TimeOut()									*
*	Description: Time left before the next update	*
*			is allowed. Returns the amount of time	*
*			in milliseconds for 30 ticks to happen.	*
****************************************************/

Uint32 SDLWin::TimeLeft(void)
{
    static Uint32 next_time = 0;		// Preinitialization. Remember that 'next_time' is static so it keeps its value
    Uint32 now;							//					even after the function has ended.

    now = SDL_GetTicks();				// Get the ammount of time since the SDL initialization
    if ( next_time <= now ) {	// If the next time to update has passed
        next_time = now+TICK_INTERVAL;	// set the next update interval
        return(0);						// return that now is the time to update
    }							// Otherwise
    return(next_time-now);				// return the time left
}

void SDLWin::To3D()
{
	//IsGLErrors("Before Switching projection");
	WM->GetRenderer()->To3D();
	Perspective=!Perspective;
}

void SDLWin::To2D()
{
	WM->GetRenderer()->To2D();
	//IsGLErrors("Before Switching projection");
	Perspective=!Perspective;
}

void SDLWin::InitTextures()
{
	TM->AddTexture("images/particle1_alpha.jpg", "White");
	TM->AddTexture("images/particle2.jpg","Particle2");
	TM->AddTexture("images/particle2_alpha.jpg","Particle2Alpha");
	TM->AddTexture("images/mouse2.jpg", "MousePointer");
	TM->AddTexture("images/mouse2_alpha.jpg", "MousePointerAlpha");
	TM->AddTexture("images/HUD/HUD.jpg", "HUD");
	TM->AddTexture("images/HUD/HUD_alpha.jpg", "HUD Alpha");
}

void SDLWin::InitGeometry()
{
}


void SDLWin::RenderPostProcessing()
{
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	float maxBlur = 256.0f;
	if((_blurY>1/maxBlur && _blurYinc==true))
	{
		float magn = (1/128.0f)/60.0f;
		float num = -rand()%1000/1000.0f*magn*1/5.0f-magn/3;
		_blurXinc = false;
		_blurYinc = false;
		_blurXrate = _blurYrate = num;
	}
	else if((_blurY<0 && _blurYinc==false))
	{
		float magn = (1/128.0f)/60.0f;
		float num = rand()%1000/1000.0f*magn/5+magn/3;
		_blurXinc = true;
		_blurYinc = true;
		_blurXrate= _blurYrate = num;
	}
	char buf3[512]={0};
	_blurX+=_blurXrate*2;
	_blurY+=_blurYrate;
	
	Plane::RenderInverted(0,0,(float)width,(float)height);
	// Apply Horizontal Blur
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	GMat->ModelMatrix()->LoadIdentity();
	WM->GetRenderer()->BindFramebuffer();
	
	WM->GetRenderer()->BindShaderProgram("HBlur");
	GLShaderProgram *shader = WM->GetRenderer()->GetCurrentShader();
	glViewport(0, 0, width, height);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurX);
	glActiveTexture(GL_TEXTURE0);
	WM->GetRenderer()->BindFramebufferTexture();
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	GMat->UpdateShader();
	Plane::RenderInverted(0,0,(float)width,(float)height);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// Apply Vertical Blur
	WM->GetRenderer()->BindShaderProgram("VBlur");
	shader = WM->GetRenderer()->GetCurrentShader();
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurY/2.0f);
	glActiveTexture(GL_TEXTURE0);
	WM->GetRenderer()->BindFramebufferTexture();
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	GMat->UpdateShader();
	Plane::RenderInverted(0,0,(float)width,(float)height);

	WM->GetRenderer()->UnbindFramebuffer();
}

void SDLWin::ModeDisplay()
{
	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("White");
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");

	char buf[255] = {0};
	if(Game->GetCurrentMode() == GM_MAINMENU)
		sprintf_s(buf, "Game Mode: Main Menu");
	else if(Game->GetCurrentMode() == GM_OPTIONSMENU)
		sprintf_s(buf, "Game Mode: Options Menu");
	else
		sprintf_s(buf, "Game Mode: Play");
	Font::Render(buf,"arial rounded",Color(255,255,255,160),24);
}
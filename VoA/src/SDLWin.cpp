#include "../include/SDLWin.h"
#include "../include/window_includes.h"
#include "../include/Plane.h"
#include "../include/Font.h"
#include <glm/gtx/transform.hpp>
#include "../include/Game.h"
#include "../include/Error.h"
/****************************************************
*	Name: SDLWin()									*
*	Description: Constructor.						*
****************************************************/

SDLWin::SDLWin()
{
	fullscreen=true;						// Initially we are not in fullscreen mode
	Perspective=true;
	_blurXrate = _blurYrate = -rand()%1000/1000.0f*(1/128.0f);
	_blurX = 1/64.0f;
	_blurY = 1.0f/128.0f;
	_blurXinc = _blurYinc = false;
	_mouseX = _mouseY = 0;
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

	OpenGLRenderer *renderer = WM->GetRenderer();
	renderer->BindShaderProgram("Main");										// Bind the main shader program
	GLShaderProgram *shader = renderer->GetCurrentShader();
	glColor4f(1.0f,1.0f,1.0f,1.0f);												// Set default color to white
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);							// Clear Color and Depth

	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	To2D();
	
	if(Game::GetCurrentMode()==GM_PLAY)
	{
		SM->RenderScene("Main");		// Render ship and exhaust
	}
	
	if(Game::GetCurrentMode()==GM_MAINMENU)
	{
		glm::mat4 tmp = glm::mat4(1.0f);
		GMat->ModelMatrix()->UpdateShader("modelMat");
	
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("Particle2");
		shader->SetUniformValue("tex",0);

		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("Particle2Alpha");
		shader->SetUniformValue("alpha",1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		WM->GetRenderer()->BindFramebuffer();
		GMat->ModelMatrix()->LoadIdentity();
		GMat->UpdateShader();
		glViewport(0, 0, width, height);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SM->RenderScene("About");
		WM->GetRenderer()->UnbindFramebuffer();

		GMat->ModelMatrix()->Translate(0,0,0);
		GMat->UpdateShader();
/*
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		char buf[512] = {0};
		Color col(255,255,255);
		sprintf_s(buf,"X: %d",_mouseX);
		Font::Render(buf,"fonts/arial rounded.ttf",col,24);
		GMat->ModelMatrix()->Translate(0.0f,20.0f,0.0f);
		GMat->ModelMatrix()->UpdateShader("modelMat");
		sprintf_s(buf, "Y: %d",_mouseY);
		Font::Render(buf,"fonts/arial rounded.ttf",col,24);*/

		glActiveTexture(GL_TEXTURE0);
		WM->GetRenderer()->BindFramebufferTexture();
		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("White");
		shader->SetUniformValue("alpha",1);
		To2D();
		GMat->ViewMatrix()->LoadIdentity();
		GMat->ModelMatrix()->PushMatrix();
		GMat->UpdateShader();

		RenderPostProcessing();

		GMat->ModelMatrix()->PopMatrix();
		GMat->UpdateShader();
	
		/*shader->Bind();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("MousePointer");
		shader->SetUniformValue("tex",0);
	
		glActiveTexture(GL_TEXTURE1);
		TM->BindTexture("MousePointerAlpha");
		shader->SetUniformValue("alpha",1);

		Plane::Render((float)_mouseX,(float)_mouseY,50.0f,50.0f);*/

		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("White");
	shader->SetUniformValue("tex",0);

	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	shader->SetUniformValue("alpha",1);

	char bu[255];
	if(WM->IsFullscreen())
		sprintf_s(bu, "Fullscreen: True");
	else
	{
		sprintf_s(bu, "Fullscreen: False");
	}

	Error::Print(bu);

	ModeDisplay();
	SDL_GL_SwapBuffers();		//Swap buffers
	//IsGLErrors("Render()");
}

/****************************************************
*	Name: Events()									*
*	Description: Handles all of the window events.	*
****************************************************/

void SDLWin::Events(SDL_Event *event)
{
    switch(event->type)					// See which event it is
    {
	case SDL_MOUSEMOTION:
	{
		_mouseX = event->motion.x;
		_mouseY = event->motion.y;
	}
    default:
		GEM->ProcessEvent(event);
        break;
	}
}

/****************************************************
*	Name: Run()										*
*	Description: This is the main loop where all	*
*			the interesting things happen			*
****************************************************/

int SDLWin::Run()
{
	Game game;
	if(WM->CreateSDLWindow() && WM->InitOpenGL())					// Initialize SDL
		Game::Run();				// If SDL initialization is successful, set the 'Running' variable for the main loop to start
	Game::SwitchMode(GM_PLAY);
	GMat->ProjectionMatrix()->SetupOrthProject();
	InitGeometry();
	InitTextures();
	WM->GetRenderer()->Init();
	To2D();

    SDL_Event Event;				// Create an event variable for catching events that SDL sends
    while(Game::IsRunning())					// Main Loop starts here
    {
        if(SDL_PollEvent(&Event)) {
            Events(&Event);			// Send Events to the 'Events' function for processing
        }
		if(Game::IsActive())
		{
			Loop();						// Game loop processing
			Render();					// Draw to the screen
			_CrtCheckMemory( );
		}
		SDL_Delay(TimeLeft());		// Limit the frame rate
    }

    //Cleanup();						// Once everything is done, make sure to clean up all loose ends
    return 1;						// Return 1 so that we know everything went fine
}

/****************************************************
*	Name: Cleanup()									*
*	Description: Cleanup all variables that need	*
*			cleaning.								*
****************************************************/

void SDLWin::Cleanup()
{
	if(Game::GetCurrentMode()==GM_QUIT)
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
	PC->ProcessKeyboardControls();
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
}

void SDLWin::InitGeometry()
{
	Entity *ent = new Entity("player");
	Ship *ship = new Ship(1.0f,"player_ship");
	float size = ship->GetSize();
	float width = ship->GetBoundingBox().x;
	float newWidth = width*size;
	ship->Init();
	ParticleSystem *lEngine = new ParticleSystem("LEngine");
	ParticleSystem *rEngine = new ParticleSystem("REngine");

	//lEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(-newWidth,-10.0,0.0)));
	TM->AddTexture("images/particle1.jpg","Particle1");
	TM->AddTexture("images/particle1_alpha.jpg","Particle1_Alpha");
	lEngine->Setup(1.0f, 7.0f, 400, 1.15f);
	lEngine->SetStrength(12.0f);
	//rEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(newWidth,-10.0,0.0)));
    rEngine->Setup(1.0f, 7.0f, 400, 1.15f);
	rEngine->SetStrength(12.0f);

	ent->AddMesh(lEngine);
	ent->AddMesh(rEngine);
	ent->AddMesh(ship);

	SM->NewScene("Main");
	SM->GetCurrentScene()->AddEntity(ent);
	SM->GetCurrentScene()->Init();

	About *about = new About("About");
	SM->AddScene(about);
	SM->GetCurrentScene()->Init();
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
    shader->SetUniformValue("alpha",1);
	GMat->UpdateShader();
	Plane::RenderInverted(0,0,(float)width,(float)width);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// Apply Vertical Blur
	WM->GetRenderer()->BindShaderProgram("VBlur");
	shader = WM->GetRenderer()->GetCurrentShader();
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurY/2.0f);
	glActiveTexture(GL_TEXTURE0);
	WM->GetRenderer()->BindFramebufferTexture();
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
    shader->SetUniformValue("alpha",1);
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
	if(Game::GetCurrentMode() == GM_MAINMENU)
		sprintf_s(buf, "Game Mode: Main Menu");
	else
		sprintf_s(buf, "Game Mode: Play");
	Font::Render(buf,"fonts/arial rounded.ttf",Color(255,255,255,160),24);
}
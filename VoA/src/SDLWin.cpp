#include "../include/SDLWin.h"
#include "../include/WindowFunctions.h"
#include "../include/Plane.h"
#include "../include/Font.h"
#include <glm/gtx/transform.hpp>
#include <SDL_ttf.h>
#include "../include/sdl_invert.h"
#include "../include/TextureManager.h"
#include "../include/Game.h"
#include "../include/WindowManager.h"
#include "../include/GlobalEventManager.h"
#include "../include/SceneManager.h"

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
	int width = WindowManager::GetSingleton()->GetWindowWidth();
	int height = WindowManager::GetSingleton()->GetWindowHeight();
	OpenGLRenderer *renderer = WindowManager::GetSingleton()->GetRenderer();
	renderer->BindShaderProgram("Main");										// Bind the main shader program
	GLShaderProgram *shader = renderer->GetCurrentShader();
	glColor4f(1.0f,1.0f,1.0f,1.0f);												// Set default color to white
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);							// Clear Color and Depth
	
	if(Game::GetCurrentMode()==GM_PLAY)
	{
		SceneManager::GetSingleton()->RenderScene();		// Render ship and exhaust
	}
	
	if(Game::GetCurrentMode()==GM_MAINMENU)
	{
		glm::mat4 tmp = glm::mat4(1.0f);
		int modelMatLoc = glGetUniformLocation(shader->GetProgramID(),"modelMat");
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);				// Send View Matrix to Shader
	
		glActiveTexture(GL_TEXTURE0);
		TextureManager::GetSingleton()->BindTexture("Particle2");
		shader->SetUniformValue("tex",0);

		glActiveTexture(GL_TEXTURE1);
		TextureManager::GetSingleton()->BindTexture("Particle2Alpha");
		shader->SetUniformValue("alpha",1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		To2D();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glViewport(0, 0, width, height);
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		about.Render();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		TTF_Font *fon = TTF_OpenFont("fonts/arial rounded.TTF",24);
		if(!fon)
		{
			char buf[512];
			sprintf_s(buf,"Error loading font: %s", TTF_GetError());
			MessageBox(NULL,buf,"Font Error!",MB_OK);
		}
		SDL_Color col;
		col.r = col.g = col.b = 255;
		glm::mat4 mat = glm::translate(glm::vec3(0.0f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		char buf[512] = {0};
		sprintf_s(buf,"X: %d",_mouseX);
		Font::Render(buf,fon,col);
		mat = glm::translate(glm::vec3(0.0f,20.0f,0.0f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
		sprintf_s(buf, "Y: %d",_mouseY);
		Font::Render(buf,fon,col);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo_texture);
		shader->SetUniformValue("tex",0);
		glActiveTexture(GL_TEXTURE1);
		TextureManager::GetSingleton()->BindTexture("White");
		shader->SetUniformValue("alpha",1);
		tmp = glm::translate(glm::vec3(0,0,0));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
		//Plane::RenderInverted(0,0,(float)width,(float)height);

		RenderPostProcessing();
	
		shader->Bind();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mat = glm::translate(glm::vec3(0.0f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
		glActiveTexture(GL_TEXTURE0);
		TextureManager::GetSingleton()->BindTexture("MousePointer");
		shader->SetUniformValue("tex",0);
	
		glActiveTexture(GL_TEXTURE1);
		TextureManager::GetSingleton()->BindTexture("MousePointerAlpha");
		shader->SetUniformValue("alpha",1);

		Plane::Render((float)_mouseX,(float)_mouseY,50.0f,50.0f);

		To3D();

		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	SDL_GL_SwapBuffers();		//Swap buffers
	IsGLErrors("Render()");
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
		GlobalEventManager::GetSingleton()->ProcessEvent(event);
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
	if(WindowManager::GetSingleton()->CreateSDLWindow() && WindowManager::GetSingleton()->InitOpenGL())					// Initialize SDL
		Game::Run();				// If SDL initialization is successful, set the 'Running' variable for the main loop to start
	Game::SwitchMode(GM_PLAY);
	about.Init();
	InitGeometry();
	InitTextures();
	InitFramebuffer();
	ShaderInit();
	SceneManager::GetSingleton()->GetCurrentScene()->Init();

    SDL_Event Event;				// Create an event variable for catching events that SDL sends
    while(Game::IsRunning())					// Main Loop starts here
    {
        if(SDL_PollEvent(&Event)) {
            Events(&Event);			// Send Events to the 'Events' function for processing
        }
        Loop();						// Game loop processing
		Render();					// Draw to the screen
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
		about.Cleanup();
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
	//Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
	//ParticleSystem *lEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("LEngine");
	//ParticleSystem *rEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("REngine");
	//float rotSpeed=10.0f;
	//// TODO: Move all processing of keystrokes to a seperate
	////		function or class for better readability
	//if(Game::GetCurrentMode()==GM_PLAY)
	//{
	//	if(keys[SDLK_RIGHT])	// If the right arrow key is pressed
	//	{
	//		ship->SetRotation(glm::vec3(0.0f,0.0f,-rotSpeed));			// Rotate the ship clockwise
	//		lEngine->SetRotation(lEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	//		rEngine->SetRotation(rEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	//	}

	//	if(keys[SDLK_UP])		// If the up arrow key is pressed
	//	{
	//		float vel = ship->GetThrottle();								// Get the ship's current throttle
	//		if(vel <= 1.75f)	// If the throttle is less than max
	//		{
	//			ship->SetThrottle(vel+.1f);								// Increase the throttle
	//			lEngine->SetStrength((vel+.1f)*(-35));
	//			rEngine->SetStrength((vel+.1f)*(-35));
	//		}
	//		glm::vec3 center = ship->GetPosition();						// Get the position of the ship
	//		// TODO: Add functionality for moving the ship around
	//	}
	//	else					// While the up key is not pressed
	//	{
	//		float vel = ship->GetThrottle();								// Get the ship's current throttle
	//		if(vel > 0.0f)		// Make sure that there is a throttle
	//		{
	//			if(vel < 0.1)	// If the throttle is close to zero
	//			{
	//				ship->SetThrottle(0);								// Go ahead and set it to zero (no need to waste fuel)
	//				lEngine->SetStrength(0);
	//				rEngine->SetStrength(0);
	//			}
	//			else
	//			{
	//				ship->SetThrottle(vel-0.05f);						// Decrease throttle
	//				lEngine->SetStrength(vel-0.05f);
	//				rEngine->SetStrength(vel-0.05f);
	//			}
	//		} 
	//	}
	//	if(keys[SDLK_DOWN])		// If down arrow key is pressed
	//	{
	//		// TODO: Figure out what to do here
	//	}
	//}

}

/****************************************************
*	Name: ShaderInit()								*
*	Description: Initializes all fragment and		*
*			vertex shaders.							*
****************************************************/

void SDLWin::ShaderInit()
{
	int width=WindowManager::GetSingleton()->GetWindowWidth();
	int height=WindowManager::GetSingleton()->GetWindowHeight();

	OpenGLRenderer *renderer = WindowManager::GetSingleton()->GetRenderer();
	renderer->NewShaderProgram("Main");	
	GLShaderProgram *shader = renderer->GetShaderProgramByName("Main");
	shader->NewFragmentShader("shaders/main.frag");			// Attach the vertex shader to the main program
	shader->NewVertexShader("shaders/main.vert");			// Attache the fragment shader to the main program
	shader->Link();											// Link the shader program
	renderer->BindShaderProgram("Main");
	
	orthMat = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -50.0f, 100.0f);	// Set the orthographic Matrix
	perspMat= glm::perspective(45.0f,(float)800/((float)600),0.1f,1000.0f);					// Set the perspective Matrix
	viewMat = glm::lookAt(glm::vec3(0.0,0.0,5.0f),glm::vec3(0,0,0),glm::vec3(0,1,0));		// Set the view matrix

	int projMatLoc = glGetUniformLocation(shader->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	int viewMatLoc = glGetUniformLocation(shader->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader
	
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&perspMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program
	
	//Blur Shader Horizontal
	renderer->NewShaderProgram("HBlur");
	shader = renderer->GetShaderProgramByName("HBlur");
	shader->NewVertexShader("shaders/main.vert");		// Load the main vertex shader from file.
	shader->NewFragmentShader("shaders/blurH.frag");	// Load the main fragment shader from file.
	shader->Link();							// Link the shader program
	renderer->BindShaderProgram("HBlur");
	
	
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurX);

	glm::mat4 modelMat = glm::translate(glm::vec3(0.0f));
	projMatLoc = glGetUniformLocation(shader->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	viewMatLoc = glGetUniformLocation(shader->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader
	int modelMatLoc = glGetUniformLocation(shader->GetProgramID(),"modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMat[0][0]);
	
	//Blur Shader Vertical
	renderer->NewShaderProgram("VBlur");
	shader = renderer->GetShaderProgramByName("VBlur");
	shader->NewVertexShader("shaders/main.vert");		// Load the main vertex shader from file.
	shader->NewFragmentShader("shaders/blurV.frag");	// Load the main fragment shader from file.
	shader->Link();										// Link the shader program
	renderer->BindShaderProgram("VBlur");
	
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurY);

	projMatLoc = glGetUniformLocation(shader->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	viewMatLoc = glGetUniformLocation(shader->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader
	modelMatLoc = glGetUniformLocation(shader->GetProgramID(),"modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMat[0][0]);

	IsGLErrors("Shader Init");
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
	int projMatLoc = glGetUniformLocation(WindowManager::GetSingleton()->GetRenderer()->GetShaderProgramByName("Main")->GetProgramID(),"projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&perspMat[0][0]);					// Send Projection Matrix to Shader	
	Perspective=!Perspective;
}

void SDLWin::To2D()
{
	//IsGLErrors("Before Switching projection");
	int projMatLoc = glGetUniformLocation(WindowManager::GetSingleton()->GetRenderer()->GetShaderProgramByName("Main")->GetProgramID(),"projMat");
	if(Perspective)
	{
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send Projection Matrix to Shader
	}
	else
	{
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&perspMat[0][0]);					// Send Projection Matrix to Shader	
	}
	Perspective=!Perspective;
}

void SDLWin::InitTextures()
{
	TextureManager::GetSingleton()->AddTexture("images/particle2.jpg","Particle2");
	TextureManager::GetSingleton()->AddTexture("images/particle2_alpha.jpg","Particle2Alpha");
	TextureManager::GetSingleton()->AddTexture("images/mouse2.jpg", "MousePointer");
	TextureManager::GetSingleton()->AddTexture("images/mouse2_alpha.jpg", "MousePointerAlpha");
	TextureManager::GetSingleton()->AddTexture("images/particle1_alpha.jpg", "White");
}

void SDLWin::InitGeometry()
{
	Entity *ent = new Entity("player");
	Ship *ship = new Ship(1.0f,"player_ship");
	float size = ship->GetSize();
	float width = ship->GetBoundingBox().x;
	float newWidth = width*size;
	ParticleSystem *lEngine = new ParticleSystem("LEngine");
	ParticleSystem *rEngine = new ParticleSystem("REngine");

	//lEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(-newWidth,-10.0,0.0)));
	lEngine->Setup(1.0f, 7.0f, 400, 1.15f);
	lEngine->SetStrength(12.0f);
	//rEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(newWidth,-10.0,0.0)));
    rEngine->Setup(1.0f, 7.0f, 400, 1.15f);
	rEngine->SetStrength(12.0f);
	ent->AddMesh(lEngine);
	ent->AddMesh(rEngine);
	ent->AddMesh(ship);

	SceneManager::GetSingleton()->NewScene("Main");
	SceneManager::GetSingleton()->GetCurrentScene()->AddEntity(ent);
	SceneManager::GetSingleton()->GetCurrentScene()->Init();
}

void SDLWin::InitFramebuffer()
{
	int width = WindowManager::GetSingleton()->GetWindowWidth();
	int height = WindowManager::GetSingleton()->GetWindowHeight();
	//glGenRenderbuffers(1, &fbo_depth);							// Generate depth buffer for frame buffer object (FBO)
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth);		// Bind depth buffer

	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window->w, window->h);
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);

	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	glGenTextures(1, &fbo_texture);					// Generate texture for frame buffer object
	glBindTexture(GL_TEXTURE_2D, fbo_texture);		// Bind the frame buffer's texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);			// Generate a standard rgba texture

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
	IsGLErrors("Framebuffer Init");
}


void SDLWin::RenderPostProcessing()
{
	int width = WindowManager::GetSingleton()->GetWindowWidth();
	int height = WindowManager::GetSingleton()->GetWindowHeight();
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

	// Apply Horizontal Blur
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	WindowManager::GetSingleton()->GetRenderer()->BindShaderProgram("HBlur");
	GLShaderProgram *shader = WindowManager::GetSingleton()->GetRenderer()->GetCurrentShader();
	glViewport(0, 0, width, height);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurX);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
    shader->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
	TextureManager::GetSingleton()->BindTexture("White");
    shader->SetUniformValue("alpha",1);
	int modelMatLoc = glGetUniformLocation(shader->GetProgramID(),"modelMat");
	glm::mat4 tmp = glm::translate(glm::vec3(0,0,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
	Plane::RenderInverted(0,0,(float)width,(float)width);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// Apply Vertical Blur
	WindowManager::GetSingleton()->GetRenderer()->BindShaderProgram("VBlur");
	shader = WindowManager::GetSingleton()->GetRenderer()->GetCurrentShader();
	glUniform1f(glGetUniformLocation(shader->GetProgramID(),"blur"),_blurY/2.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
    shader->SetUniformValue("tex",0);
	modelMatLoc = glGetUniformLocation(shader->GetProgramID(),"modelMat");
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
	Plane::RenderInverted(0,0,(float)width,(float)height);
}

void SDLWin::TurnLeft()
{
	//Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
	//ParticleSystem *lEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("LEngine");
	//ParticleSystem *rEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("REngine");
	//float rotSpeed=10.0f;											// Set a speed for the rotation of the 

	//ship->SetRotation(glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship counter clockwise
	//lEngine->SetRotation(lEngine->GetRotation()+glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship counter clockwise
	//rEngine->SetRotation(rEngine->GetRotation()+glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship counter clockwise
}

void SDLWin::TurnRight()
{
	//Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
	//ParticleSystem *lEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("LEngine");
	//ParticleSystem *rEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("REngine");
	//float rotSpeed=10.0f;

	//ship->SetRotation(glm::vec3(0.0f,0.0f,-rotSpeed));			// Rotate the ship clockwise
	//lEngine->SetRotation(lEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	//rEngine->SetRotation(rEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
}

void SDLWin::InitKeyActions()
{
	//KeyAction kAct;
	//kAct.Key = SDLK_LEFT;
	//kAct.name = "TurnLeft";
	//kAct.State = KS_DOWN;
	///*typedef void(SDLWin::*point)(void) const;
	//point fp = this->TurnLeft;*/
	////kAct.Action = fp;
	//KeyHandler::GetSingleton()->AddAction(kAct);

	//kAct.Key = SDLK_RIGHT;
	//kAct.name = "TurnRight";
	//kAct.State = KS_DOWN;
	//kAct.Action = this->TurnRight;
	//Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
	//ParticleSystem *lEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("LEngine");
	//ParticleSystem *rEngine = (ParticleSystem *)scene1.GetEntityByName("player")->GetMeshByName("REngine");
	//float rotSpeed=10.0f;
																	// TODO: Check why the rotation is backwards	
	//if(keys[SDLK_RIGHT])	// If the right arrow key is pressed
	//{
	//	ship->SetRotation(glm::vec3(0.0f,0.0f,-rotSpeed));			// Rotate the ship clockwise
	//	lEngine->SetRotation(lEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	//	rEngine->SetRotation(rEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	//}

	//if(keys[SDLK_UP])		// If the up arrow key is pressed
	//{
	//	float vel = ship->GetThrottle();								// Get the ship's current throttle
	//	if(vel <= 1.75f)	// If the throttle is less than max
	//	{
	//		ship->SetThrottle(vel+.1f);								// Increase the throttle
	//		lEngine->SetStrength((vel+.1f)*(-35));
	//		rEngine->SetStrength((vel+.1f)*(-35));
	//	}
	//	glm::vec3 center = ship->GetPosition();						// Get the position of the ship
	//	// TODO: Add functionality for moving the ship around
	//}
	//else					// While the up key is not pressed
	//{
	//	float vel = ship->GetThrottle();								// Get the ship's current throttle
	//	if(vel > 0.0f)		// Make sure that there is a throttle
	//	{
	//		if(vel < 0.1)	// If the throttle is close to zero
	//		{
	//			ship->SetThrottle(0);								// Go ahead and set it to zero (no need to waste fuel)
	//			lEngine->SetStrength(0);
	//			rEngine->SetStrength(0);
	//		}
	//		else
	//		{
	//			ship->SetThrottle(vel-0.05f);						// Decrease throttle
	//			lEngine->SetStrength(vel-0.05f);
	//			rEngine->SetStrength(vel-0.05f);
	//		}
	//	} 
	//}
	//if(keys[SDLK_DOWN])		// If down arrow key is pressed
	//{
	//	// TODO: Figure out what to do here
	//}
}
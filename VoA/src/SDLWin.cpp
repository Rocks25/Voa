#include "../include/SDLWin.h"
#include "../include/myglext.h"
#include "../include/Plane.h"
#include "../include/Font.h"
#include <glm\gtx\transform.hpp>
#include <SDL_ttf.h>
#include "../include/sdl_invert.h"

/****************************************************
*	Name: SDLWin()									*
*	Description: Constructor.						*
****************************************************/

SDLWin::SDLWin()
{
	memset(keys,false,sizeof(keys));		// set all keys in the 'keys' variable to be unpressed
	fullscreen=true;						// Initially we are not in fullscreen mode
	Perspective=true;
	grabmouse=true;
	_blurXrate = _blurYrate = -rand()%1000/1000.0f*(1/128.0f);
	_blurX = _blurY = 1.0f/128.0f;
	_blurXinc = _blurYinc = false;
	spMain = new GLShaderProgram();
	spBlurH = new GLShaderProgram();
	spBlurV = new GLShaderProgram();
}

/****************************************************
*	Name: ~SDLWin()									*
*	Description: Destructor.						*
****************************************************/

SDLWin::~SDLWin()
{
	delete spBlurH;
	delete spBlurV;
	delete spMain;
    Cleanup();
}

/****************************************************
*	Name: glInit()									*
*	Description: Initializes OpenGL options.		*
****************************************************/

void SDLWin::glInit()
{
    glShadeModel(GL_SMOOTH);								// Set the rendering of models, if we have any, to be smooth
    glClearColor(0.0f,0.0f,0.0f,0.0f);						// Set the clear color to be black
    glClearDepth(1.0f);                                     // Setup the Depth Buffer
	//glEnable(GL_CULL_FACE);									// Enable culling of faces
	//glCullFace(GL_BACK);									// Make sure not to draw the back of faces by default
	
    glDisable(GL_DEPTH_TEST);								// Because we are blending, we don't want it to originally check for depth
    glEnable(GL_BLEND);                                     // Enable Blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Type Of Blending To Perform
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);       // Nicest Perspective Calculations
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);                 // Nicest Point Smoothing
	glewInit();												// Initialize GL Extensions
	about.Init(window);

	checkGLErrors("GL Init");
}

/****************************************************
*	Name: Render()									*
*	Description: Draws everything to the screen.	*
****************************************************/

void SDLWin::Render()
{
	spMain->Bind();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);							// Clear Color and Depth
	
	//checkGLErrors("Before Model Matrix ");
	//scene1.Render(&spMain);		// Render ship and exhaust
	glm::mat4 tmp = glm::mat4(1.0f);
	int modelMatLoc = glGetUniformLocation(spMain->GetProgramID(),"modelMat");
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);				// Send View Matrix to Shader
	
	//checkGLErrors("After Model Matrix ");
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ptexture[0]);
    spMain->SetUniformValue("tex",0);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ptexture[1]);
    spMain->SetUniformValue("alpha",1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	ToggleProjectionType();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glViewport(0, 0, window->w, window->h);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	about.Render(spMain);
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
    spMain->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,ptexture[3]);
    spMain->SetUniformValue("alpha",1);
	tmp = glm::translate(glm::vec3(0,0,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
	Plane::RenderInverted(0,0,(float)window->w,(float)window->h);

	RenderPostProcessing();
	
	spMain->Bind();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mat = glm::translate(glm::vec3(0.0f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ptexture[2]);
    spMain->SetUniformValue("tex",0);
	
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ptexture[4]);
    spMain->SetUniformValue("alpha",1);

	Plane::Render((float)_mouseX,(float)_mouseY,50.0f,50.0f);

	ToggleProjectionType();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    SDL_GL_SwapBuffers();		//Swap buffers
	char buf2[2048] = {0};
	int length = 0;
	glGetProgramInfoLog(spBlurH->GetProgramID(),2048,&length,buf2);
	//glGetProgramInfoLog(spBlurV->GetProgramID(),2048,&length,buf2);
	if(strstr(buf2, "No errors")==0 && buf2!="")
		MessageBox(NULL, buf2, "INFO", MB_OK);
	checkGLErrors("Render()");
}

/****************************************************
*	Name: Events()									*
*	Description: Handles all of the window events.	*
****************************************************/

void SDLWin::Events(SDL_Event *event)
{
    switch(event->type)					// See which event it is
    {
		// When window is resized or is toggled between window and fullscreen
	case SDL_VIDEORESIZE:
		glViewport(0,0,event->resize.w,event->resize.h);		// Resize OpenGL Viewport to fit the screen/window
		glInit();						// Reinitialize OpenGL options
		ShaderInit();
		InitFramebuffer();
		//scene1.GetEntityByName("player")->GetMeshByName("player_ship")->Init();			// Reinitialize Textures
		about.Resize(event->resize.w,event->resize.h);
		InitTextures();
		break;
		// When user quits the game
    case SDL_QUIT:
        Running = false;				// End main loop
        break;
		// When a key is pressed
    case SDL_KEYDOWN:
    {
        switch(event->key.keysym.sym)
        {
		case 'm':
			if(grabmouse)
			{
				SDL_WM_GrabInput( SDL_GRAB_OFF );
				SDL_ShowCursor(SDL_ENABLE);
			}
			else
			{
				SDL_WM_GrabInput( SDL_GRAB_ON );
				SDL_ShowCursor(SDL_DISABLE);
			}
			grabmouse=!grabmouse;
			break;
        case 'q':
        case SDLK_ESCAPE:
            Running=false;				// End main loop
            break;
		case 'f':				// If 'f' key was pressed
			if(fullscreen)		// Check if we are already in fullscreen, and if we are
			{
				window=SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_RESIZABLE);		// Switch to windowed
			}
			else				// Otherwise
			{
				window=SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL|SDL_FULLSCREEN);		// Switch to Fullscreen

			}
			fullscreen=!fullscreen;		// toggle fullscreen variable
			break;
        default:	// For all other keys
			keys[event->key.keysym.sym]=true;			// Update global 'keys' variable
            break;
        }
		break;
    }
	// When a key is released
	case SDL_KEYUP:
	{
		keys[event->key.keysym.sym]=false;				// Update global 'keys' variable
	}
	case SDL_MOUSEMOTION:
	{
		_mouseX = event->motion.x;
		_mouseY = event->motion.y;
	}
    default:
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
    Running = false;				// Preinitialize Running variable to make sure it is false
    if(SDLInit())					// Initialize SDL
        Running = true;				// If SDL initialization is successful, set the 'Running' variable for the main loop to start
    glInit();						// Initialize OpenGL
	//scene1.Init();					// TODO: For testing, will remove later once the Player and Scene classes are implemented

	InitTextures();
	InitGeometry();
	ShaderInit();
	InitFramebuffer();

    SDL_Event Event;				// Create an event variable for catching events that SDL sends
    while(Running)					// Main Loop starts here
    {
        if(SDL_PollEvent(&Event)) {
            Events(&Event);			// Send Events to the 'Events' function for processing
        }
        Loop();						// Game loop processing
		Render();					// Draw to the screen
		SDL_Delay(TimeLeft());		// Limit the frame rate
    }

    Cleanup();						// Once everything is done, make sure to clean up all loose ends
    return 1;						// Return 1 so that we know everything went fine
}

/****************************************************
*	Name: Cleanup()									*
*	Description: Cleanup all variables that need	*
*			cleaning.								*
****************************************************/

void SDLWin::Cleanup()
{
	std::vector<GLShader *> shaders = spMain->GetShaders();			// Retrieve the 'shaders' vector from the shader program
	for(unsigned int i=0;i<shaders.size();i++)								// Cycle through each shader and disable it
	{
		GLShader *shader = shaders[i];
		shader->~GLShader();
	}
    SDL_Quit();														// Close Out
}

/****************************************************
*	Name: Loop()									*
*	Description: Do all calculations before the		*
*			Render function draws to the screen		*
****************************************************/

void SDLWin::Loop()
{

	// TODO: Move all processing of keystrokes to a seperate
	//		function or class for better readability

	float rotSpeed=10.0f;											// Set a speed for the rotation of the 
	if(keys[SDLK_LEFT])		// If the left arrow key is pressed
	{
		Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
		ship->SetRotation(glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship counter clockwise
	}
																	// TODO: Check why the rotation is backwards	
	if(keys[SDLK_RIGHT])	// If the right arrow key is pressed
	{
		Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
		ship->SetRotation(glm::vec3(0.0f,0.0f,-rotSpeed));			// Rotate the ship clockwise
	}

	if(keys[SDLK_UP])		// If the up arrow key is pressed
	{
		Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
		float vel = ship->GetThrottle();								// Get the ship's current throttle
		if(vel <= 1.75f)	// If the throttle is less than max
		{
			ship->SetThrottle(vel+.1f);								// Increase the throttle
		}
		glm::vec3 center = ship->GetPosition();						// Get the position of the ship
		// TODO: Add functionality for moving the ship around
	}
	//else					// While the up key is not pressed
	//{
	//	Ship *ship = (Ship *)scene1.GetEntityByName("player")->GetMeshByName("player_ship");
	//	float vel = ship->GetThrottle();								// Get the ship's current throttle
	//	if(vel > 0.0f)		// Make sure that there is a throttle
	//	{
	//		if(vel < 0.1)	// If the throttle is close to zero
	//		{
	//			ship->SetThrottle(0);								// Go ahead and set it to zero (no need to waste fuel)
	//		}
	//		else
	//			ship->SetThrottle(vel-0.05f);						// Decrease throttle
	//	} 
	//}
	if(keys[SDLK_DOWN])		// If down arrow key is pressed
	{
		// TODO: Figure out what to do here
	}

}

/****************************************************
*	Name: SDLInit()									*
*	Description: Initialize SDL						*
****************************************************/

bool SDLWin::SDLInit()
{								// Create a new SDL surface to draw on
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {						// Initialize everything
        return false;											// or return that there was an error
    }

	if((window = SDL_SetVideoMode(1680, 1050, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN)) == NULL) {
        return false;												// Set the window size to 800x600, tell it that we want it to by an OpenGL window
    }																//			and resizable, or return that there has been an error
										// Point the global window variable to the new SDL surface
	SDL_WM_GrabInput( SDL_GRAB_ON );
	SDL_ShowCursor(SDL_DISABLE);

	TTF_Init();

    return true;													// return that everything went alright
}

/****************************************************
*	Name: ShaderInit()								*
*	Description: Initializes all fragment and		*
*			vertex shaders.							*
****************************************************/

void SDLWin::ShaderInit()
{
	GLShader *vsMain = new GLShader(GL_VERTEX_SHADER,"shaders/main.vert");		// Load the main vertex shader from file.
	GLShader *fsMain = new GLShader(GL_FRAGMENT_SHADER,"shaders/main.frag");	// Load the main fragment shader from file.
	vsMain->Init();						// Initialize and compile the vertex shader
	fsMain->Init();						// Initialize and compile the fragment shader
	spMain->Create();						// Create the main shader program
	spMain->AttachShader(vsMain);			// Attach the vertex shader to the main program
	spMain->AttachShader(fsMain);			// Attache the fragment shader to the main program
	spMain->Link();							// Link the shader program

	spMain->Bind();							// Use the main program now
	
	orthMat = glm::ortho(0.0f, (float)window->w, (float)window->h, 0.0f, -50.0f, 100.0f);	// Set the orthographic Matrix
	perspMat= glm::perspective(45.0f,(float)800/((float)600),0.1f,1000.0f);					// Set the perspective Matrix
	viewMat = glm::lookAt(glm::vec3(0.0,0.0,5.0f),glm::vec3(0,0,0),glm::vec3(0,1,0));		// Set the view matrix

	int projMatLoc = glGetUniformLocation(spMain->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	int viewMatLoc = glGetUniformLocation(spMain->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&perspMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program


	//Blur Shader Horizontal
	GLShader *vsBlurH = new GLShader(GL_VERTEX_SHADER,"shaders/main.vert");		// Load the main vertex shader from file.
	GLShader *fsBlurH = new GLShader(GL_FRAGMENT_SHADER,"shaders/blurH.frag");	// Load the main fragment shader from file.
	vsBlurH->Init();						// Initialize and compile the vertex shader
	fsBlurH->Init();						// Initialize and compile the fragment shader
	spBlurH->Create();						// Create the main shader program
	spBlurH->AttachShader(vsBlurH);			// Attach the vertex shader to the main program
	spBlurH->AttachShader(fsBlurH);			// Attache the fragment shader to the main program
	spBlurH->Link();							// Link the shader program

	spBlurH->Bind();
	
	glUniform1f(glGetUniformLocation(spBlurH->GetProgramID(),"blur"),_blurX);

	glm::mat4 modelMat = glm::translate(glm::vec3(0.0f));
	projMatLoc = glGetUniformLocation(spBlurH->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	viewMatLoc = glGetUniformLocation(spBlurH->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader
	int modelMatLoc = glGetUniformLocation(spBlurH->GetProgramID(),"modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMat[0][0]);

	//Blur Shader Vertical
	GLShader *vsBlurV = new GLShader(GL_VERTEX_SHADER,"shaders/main.vert");		// Load the main vertex shader from file.
	GLShader *fsBlurV = new GLShader(GL_FRAGMENT_SHADER,"shaders/blurV.frag");	// Load the main fragment shader from file.
	vsBlurV->Init();						// Initialize and compile the vertex shader
	fsBlurV->Init();						// Initialize and compile the fragment shader
	spBlurV->Create();						// Create the main shader program
	spBlurV->AttachShader(vsBlurV);			// Attach the vertex shader to the main program
	spBlurV->AttachShader(fsBlurV);			// Attache the fragment shader to the main program
	spBlurV->Link();							// Link the shader program

	spBlurV->Bind();
	
	glUniform1f(glGetUniformLocation(spBlurV->GetProgramID(),"blur"),_blurY);

	projMatLoc = glGetUniformLocation(spBlurV->GetProgramID(),"projMat");	// Get the location of the projection matrix from shader
	viewMatLoc = glGetUniformLocation(spBlurV->GetProgramID(),"viewMat");	// Get the location of the view matrix from the shader
	modelMatLoc = glGetUniformLocation(spBlurV->GetProgramID(),"modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send the perspective matrix to the shader program
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMat[0][0]);				// Send the view matrix to the shader program
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMat[0][0]);

	checkGLErrors("Shader Init");
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

void SDLWin::ToggleProjectionType()
{
	//checkGLErrors("Before Switching projection");
	int projMatLoc = glGetUniformLocation(spMain->GetProgramID(),"projMat");
	if(Perspective)
	{
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&orthMat[0][0]);					// Send Projection Matrix to Shader
	}
	else
	{
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,&perspMat[0][0]);					// Send Projection Matrix to Shader	
	}
	Perspective=!Perspective;
	
	//checkGLErrors("After Switching projection");
}

void SDLWin::InitTextures()
{
	glGenTextures(5,ptexture);
    SDL_Surface *a = IMG_Load("images/particle2.jpg");
	SDL_PixelFormat *format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
	SDL_InvertSurface(a);
    glBindTexture( GL_TEXTURE_2D, ptexture[0] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);

	SDL_Surface *b = IMG_Load("images/particle2_alpha.jpg");
	format = b->format;
	if(!b)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
	SDL_InvertSurface(b);
    glBindTexture( GL_TEXTURE_2D, ptexture[1] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGB, GL_UNSIGNED_BYTE, b->pixels );
	
	SDL_FreeSurface(b);

	b = IMG_Load("images/mouse2.jpg");
	format = b->format;
	if(!b)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, ptexture[2] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGB, GL_UNSIGNED_BYTE, b->pixels );
	
	SDL_FreeSurface(b);

	b = IMG_Load("images/particle1_alpha.jpg");
	format = b->format;
	if(!b)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, ptexture[3] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGB, GL_UNSIGNED_BYTE, b->pixels );
	
	SDL_FreeSurface(b);

	b = IMG_Load("images/mouse2_alpha.jpg");
	format = b->format;
	if(!b)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, ptexture[4] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGB, GL_UNSIGNED_BYTE, b->pixels );
	
	SDL_FreeSurface(b);
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

	lEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(-newWidth,-10.0,0.0)));
	lEngine->Init(1.0f, 7.0f, 400, 1.15f);
	lEngine->SetStrength(12.0f);
	rEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(newWidth,-10.0,0.0)));
    rEngine->Init(1.0f, 7.0f, 400, 1.15f);
	rEngine->SetStrength(12.0f);
	ent->AddMesh(ship);
	ent->AddMesh(lEngine);
	ent->AddMesh(rEngine);

	scene1.AddEntity(ent);

	scene1.Init();
}

void SDLWin::InitFramebuffer()
{
	//glGenRenderbuffers(1, &fbo_depth);							// Generate depth buffer for frame buffer object (FBO)
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth);		// Bind depth buffer

	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window->w, window->h);
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);

	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	glGenTextures(1, &fbo_texture);					// Generate texture for frame buffer object
	glBindTexture(GL_TEXTURE_2D, fbo_texture);		// Bind the frame buffer's texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->w, window->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);			// Generate a standard rgba texture

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
}


void SDLWin::RenderPostProcessing()
{
	if((_blurY>1/128.0f && _blurYinc==true)|| (_blurX>1/128.0f && _blurXinc==true))
	{
		float magn = (1/128.0f)/60.0f;
		float num = -rand()%1000/1000.0f*magn*1/10.0f-magn/3;
		_blurXinc = false;
		_blurYinc = false;
		_blurXrate = _blurYrate = num;
	}
	else if((_blurY<0 && _blurYinc==false)||(_blurX<0 && _blurXinc==false))
	{
		float magn = (1/128.0f)/60.0f;
		float num = rand()%1000/1000.0f*magn/10+magn/3;
		_blurXinc = true;
		_blurYinc = true;
		_blurXrate= _blurYrate = num;
	}
	char buf3[512]={0};
	_blurX+=_blurXrate;
	_blurY+=_blurYrate;

	// Apply Horizontal Blur
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	spBlurH->Bind();
	glViewport(0, 0, window->w, window->h);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glUniform1f(glGetUniformLocation(spBlurH->GetProgramID(),"blur"),_blurX);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
    spBlurH->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,ptexture[3]);
    spBlurH->SetUniformValue("alpha",1);
	int modelMatLoc = glGetUniformLocation(spBlurH->GetProgramID(),"modelMat");
	glm::mat4 tmp = glm::translate(glm::vec3(0,0,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
	Plane::RenderInverted(0,0,(float)window->w,(float)window->h);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// Apply Vertical Blur
	spBlurV->Bind();
	glUniform1f(glGetUniformLocation(spBlurV->GetProgramID(),"blur"),_blurY/2.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
    spBlurV->SetUniformValue("tex",0);
	modelMatLoc = glGetUniformLocation(spBlurV->GetProgramID(),"modelMat");
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &tmp[0][0]);
	Plane::RenderInverted(0,0,(float)window->w,(float)window->h);
}
#include "..\include\PlayerController.h"
#include "..\include\Game.h"
#include "..\include\ship.h"
#include "..\include\particlesystem.h"
#include "..\include\Scene.h"

PlayerController::PlayerController(void)
{
}


PlayerController::~PlayerController(void)
{
}

void PlayerController::ProcessKeyboardEvent(SDL_Event *event)
{
	Entity *ent = SM->GetSceneByName("Main")->GetEntityByName("player");
	SM->BindScene("Main");
	Ship *ship = (Ship *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("player_ship");
	ParticleSystem *lEngine = (ParticleSystem *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("LEngine");
	ParticleSystem *rEngine = (ParticleSystem *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("REngine");
	if(!ship || !lEngine || !rEngine)
		return;
	float rotSpeed=10.0f;
	float vel = ship->GetThrottle();								// Get the ship's current throttle
	switch(event->type)
	{
	case SDL_KEYUP:
		keys[event->key.keysym.sym] = false;
		break;
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
				Game::SwitchMode(GM_MAINMENU);
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
		case 'q':
			Game::Quit();				// End main loop
			Game::SwitchMode(SDL_QUIT);
			break;
		case 'f':				// If 'f' key was pressed
			WM->ToggleFullscreen();
			break;
		default:
			keys[event->key.keysym.sym] = true;
			break;
		}
	default:
		break;
	}
}

void PlayerController::ProcessKeyboardControls()
{
	Entity *ent = SM->GetSceneByName("Main")->GetEntityByName("player");
	SM->BindScene("Main");
	Ship *ship = (Ship *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("player_ship");
	ParticleSystem *lEngine = (ParticleSystem *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("LEngine");
	ParticleSystem *rEngine = (ParticleSystem *)SM->GetCurrentScene()->GetEntityByName("player")->GetMeshByName("REngine");
	if(!ship || !lEngine || !rEngine)
		return;
	float rotSpeed=10.0f;
	float vel = ship->GetThrottle();

	if(keys[SDLK_RIGHT]) // If the right arrow key is pressed
	{
		ship->SetRotation(glm::vec3(0.0f,0.0f,+rotSpeed));			// Rotate the ship clockwise
		lEngine->SetRotation(lEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
		rEngine->SetRotation(rEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	}
	
	if(keys[SDLK_LEFT]) // If the right arrow key is pressed
	{
		ship->SetRotation(glm::vec3(0.0f,0.0f,-rotSpeed));									// Rotate the ship clockwise
		lEngine->SetRotation(lEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
		rEngine->SetRotation(rEngine->GetRotation()-glm::vec3(0.0f,0.0f,rotSpeed));			// Rotate the ship clockwise
	}

	if(keys[SDLK_UP])
	{
		if(vel <= 1.75f)	// If the throttle is less than max
		{
			ship->SetThrottle(vel+.1f);								// Increase the throttle
			lEngine->SetStrength((vel+.1f)*(-35));
			rEngine->SetStrength((vel+.1f)*(-35));
		}
		// TODO: Add functionality for moving the ship around
	}
	else
	{
		// While the up key is not pressed
		if(vel > 0.0f)		// Make sure that there is a throttle
		{
			if(vel < 0.1)	// If the throttle is close to zero
			{
				ship->SetThrottle(0);								// Go ahead and set it to zero (no need to waste fuel)
				lEngine->SetStrength(0);
				rEngine->SetStrength(0);
			}
			else
			{
				ship->SetThrottle(vel-0.05f);						// Decrease throttle
				lEngine->SetStrength(vel-0.05f);
				rEngine->SetStrength(vel-0.05f);
			}
		}
	}
}
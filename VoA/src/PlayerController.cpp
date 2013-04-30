#include "..\include\PlayerController.h"
#include "..\include\Game.h"
#include "..\include\ship.h"
#include "..\include\particlesystem.h"
#include "..\include\Plane.h"
#include "..\include\Scene.h"
#include "..\include\SoundManager.h"
#include <algorithm>

PlayerController::PlayerController(void)
{
	memset(keys, 0,sizeof(keys));
}


PlayerController::~PlayerController(void)
{
}

void PlayerController::ProcessControlEvent(SDL_Event *event)
{
	switch(event->type)
	{
	case SDL_MOUSEMOTION:
		mouse_x = event->motion.x;
		mouse_y = event->motion.y;
		break;
	case SDL_KEYUP:		
		switch(event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			Game->Pause();
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
		default:
			keys[event->key.keysym.sym] = false;
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
				Game->Pause();
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
		default:
			keys[event->key.keysym.sym] = true;
			break;
		}
		break;
	default:
		break;
	}
	if(event->type == SDL_KEYDOWN)
	{
		if(event->key.keysym.sym == 'w')
		{
			Sounds->StartSound("Thrusters",0,-1);
		}
	}
	else if(event->type == SDL_KEYUP)
	{
		if(event->key.keysym.sym == 'w')
		{
			Sounds->StopSound("Thrusters",1000);
		}
	}
}

void PlayerController::ProcessControls()
{
	if(Game->GetCurrentMode() == GM_PLAY)
	{
		Ship *ship = (Ship *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("Player Ship");
		Plane *lengine = (Plane *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("LEngine");
		Plane *rengine = (Plane *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("REngine");
		float magnitude = sqrtf(pow(GetMouseX()-ship->GetPosition().x,2)+pow(GetMouseY()-ship->GetPosition().y,2));
		glm::vec3 dir = glm::vec3((GetMouseX()-ship->GetPosition().x),(GetMouseY()-ship->GetPosition().y),0)/magnitude;
		float rotSpeed=10.0f;
		float maxspeed = 12.0f;
		float acceleration = 0.5f;
		glm::vec3 vel = ship->GetThrottle();
		float accelerating = lengine->GetScale().x;
	
		if(keys[SDLK_RIGHT]) // If the right arrow key is pressed
		{
			ship->Rotate(glm::vec3(0.0f,0.0f,+rotSpeed));			// Rotate the ship clockwise
		}
	
		if(keys[SDLK_LEFT]) // If the right arrow key is pressed
		{
			ship->Rotate(glm::vec3(0.0f,0.0f,-rotSpeed));									// Rotate the ship clockwise
		}

		if(keys['w'])
		{
			if(magnitude > -maxspeed)	// If the throttle is less than max
			{
				vel += 0.5f*dir*acceleration;
			}
			else
			{
				vel -= maxspeed;
			}

			if(accelerating < 1.0f)
				accelerating += .05f;
			else
				accelerating = 1.0f;
		}
		else
		{
			if(accelerating > 0)
				accelerating -= .05f;
			else
				accelerating = 0.0f;
		}

		if(vel.x > 0 && vel.x < 0.1f)	// If the throttle is close to zero
		{
			vel.x=0;											// Go ahead and set it to zero (no need to waste fuel)
		}
		if(vel.y > 0 && vel.y < 0.1f)	// If the throttle is close to zero
		{
			vel.y=0;											// Go ahead and set it to zero (no need to waste fuel)
		}
		
		if(vel.x < 0 && vel.x > -0.1f)	// If the throttle is close to zero
		{
			vel.x=0;											// Go ahead and set it to zero (no need to waste fuel)
		}
		if(vel.y < 0 && vel.y > -0.1f)	// If the throttle is close to zero
		{
			vel.y=0;											// Go ahead and set it to zero (no need to waste fuel)
		}
		
		glm::vec3 pos = ship->GetPosition();
		
		if(pos.x > WM->GetWindowWidth()+16.0f)
		{
			pos.x = -16.0f;
			ship->SetPosition(pos);
		}
		else if(pos.x < -16.0f)
		{
			pos.x = WM->GetWindowWidth()+16.0f;
			ship->SetPosition(pos);
		}

		if(pos.y > WM->GetWindowHeight()+16.0f)
		{
			pos.y = -16.0f;
			ship->SetPosition(pos);
		}
		else if(pos.y < -16.0f)
		{
			pos.y = WM->GetWindowHeight()+16.0f;
			ship->SetPosition(pos);
		}

		ship->SetThrottle(vel);
		ship->Translate(vel);
		pos = ship->GetPosition();

		glm::vec3 rot = glm::vec3(0,0,-(atan2(mouse_x-pos.x,mouse_y-pos.y)+3.14159)*180/3.14159);
		ship->SetRotation(rot);

		if(!lengine || !rengine)
			return;
		lengine->SetOrigin(-50,0);
		lengine->SetPosition(pos);
		lengine->SetRotation(rot);
		float lscalemod = rand()%10/100.0f-0.05f;
		float rscalemod = rand()%10/100.0f-0.05f;
		glm::vec3 lscale = glm::vec3(accelerating+lscalemod,accelerating+lscalemod,accelerating+lscalemod);
		glm::vec3 rscale = glm::vec3(accelerating+rscalemod,accelerating+rscalemod,accelerating+rscalemod);
		lengine->SetScale(lscale);
		rengine->SetScale(rscale);
		rengine->SetPosition(pos);
		rengine->SetRotation(rot);
	}
}

int PlayerController::GetMouseX()
{
	return mouse_x;
}

int PlayerController::GetMouseY()
{
	return mouse_y;
}
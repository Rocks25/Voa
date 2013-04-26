#include "..\include\PlayerController.h"
#include "..\include\Game.h"
#include "..\include\ship.h"
#include "..\include\particlesystem.h"
#include "..\include\Scene.h"

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
		keys[event->key.keysym.sym] = false;
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
	default:
		break;
	}
}

void PlayerController::ProcessControls()
{
	if(Game->GetCurrentMode() == GM_PLAY)
	{
		Ship *ship = (Ship *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("Player Ship");
		float rotSpeed=10.0f;
		float maxspeed = 5.0f;
		float acceleration = 0.5f;
		glm::vec3 vel = ship->GetThrottle();
	
		if(keys[SDLK_RIGHT]) // If the right arrow key is pressed
		{
			ship->Rotate(glm::vec3(0.0f,0.0f,+rotSpeed));			// Rotate the ship clockwise
		}
	
		if(keys[SDLK_LEFT]) // If the right arrow key is pressed
		{
			ship->Rotate(glm::vec3(0.0f,0.0f,-rotSpeed));									// Rotate the ship clockwise
		}

		if(keys['a'])
		{
			if(vel.x > -maxspeed)	// If the throttle is less than max
			{
				vel.x -= 0.5f*acceleration;
			}
			else
			{
				vel.x = -maxspeed;
			}
		}

		if(keys['d'])
		{
			if(vel.x < maxspeed)	// If the throttle is less than max
			{
				vel.x += 0.5f*acceleration;
			}
			else
			{
				vel.x = maxspeed;
			}
		}

		if(!keys['a'] && !keys['d'])
		{
			if(vel.x > 0)
				vel.x -= 0.05f;
			else if(vel.x < 0)
				vel.x += 0.05f;
		}

		if(keys['w'])
		{
			if(vel.y > -maxspeed)	// If the throttle is less than max
			{
				vel.y -= 0.5f*acceleration;
			}
			else
			{
				vel.y = -maxspeed;
			}
		}

		if(keys['s'])
		{
			if(vel.y < maxspeed)	// If the throttle is less than max
			{
				vel.y += 0.5f*acceleration;
			}
			else
			{
				vel.y = maxspeed;
			}
		}

		if(!keys['w'] && !keys['s'])
		{
			if(vel.y > 0)
				vel.y -= 0.05f;
			else if(vel.y < 0)
				vel.y += 0.05f;
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
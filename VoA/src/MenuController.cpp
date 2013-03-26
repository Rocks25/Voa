#include "..\include\MenuController.h"
#include "..\include\Game.h"
#include "..\include\WindowManager.h"


MenuController::MenuController(void)
{
}


MenuController::~MenuController(void)
{
}

void MenuController::ProcessKeyboardEvents(SDL_Event *event)
{
	switch(event->type)
	{
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
        {
		case SDLK_F1:
			if(Game::GetCurrentMode()==GM_PLAY)
				Game::SwitchMode(GM_MAINMENU);
			else
				Game::SwitchMode(GM_PLAY);
			break;
		case 'm':
			if(WindowManager::GetSingleton()->IsMouseGrabbed())
				WindowManager::GetSingleton()->ReleaseMouse();
			else
				WindowManager::GetSingleton()->GrabMouse();
			break;
        case 'q':
        case SDLK_ESCAPE:
			Game::Quit();				// End main loop
			Game::SwitchMode(SDL_QUIT);
            break;
		case 'f':				// If 'f' key was pressed
			WindowManager::GetSingleton()->ToggleFullscreen();
			break;
        default:
            break;
        }
		break;
	case SDL_KEYUP:
		break;
	default:
		break;
	}
}
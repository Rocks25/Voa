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
		case SDLK_ESCAPE:
			Game::SwitchMode(GM_PLAY);
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
        case 'q':
			Game::Quit();				// End main loop
			Game::SwitchMode(GM_QUIT);
            break;
		case 'f':				// If 'f' key was pressed
			WM->ToggleFullscreen();
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
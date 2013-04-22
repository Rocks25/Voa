#include "..\include\MenuController.h"
#include "..\include\Game.h"
#include "..\include\WindowManager.h"
#include "..\include\MainMenu.h"
#include "..\include\OptionsMenu.h"


MenuController::MenuController(void)
{
}


MenuController::~MenuController(void)
{
}

void MenuController::ProcessKeyboardEvents(SDL_Event *event)
{
	if(Game->GetCurrentMode() == GM_MAINMENU)
		ProcessMainMenu(event);
	else
		ProcessOptionsMenu(event);
}

void MenuController::ProcessMainMenu(SDL_Event *event)
{
	MainMenu *menu = (MainMenu *)SM->GetSceneByName("Main Menu");
	int selected = menu->GetSelection();

	switch(event->type)
	{
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
        {
		case SDLK_ESCAPE:
			Game->SwitchMode(GM_PLAY);
			menu->SetSelection(0);
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
		case 'f':				// If 'f' key was pressed
			WM->ToggleFullscreen();
			break;
		case SDLK_DOWN:
			menu->SelectNext();
			break;
		case SDLK_UP:
			menu->SelectPrev();
			break;
		case SDLK_RETURN:
			if(Game->HasGameStarted())
			{
				if(selected==0)
				{
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->SwitchMode(GM_PLAY);			// Back to Game
				}
				else if(selected==1)
				{
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->StartNewGame();				// Start a new Game
				}
				else if(selected==2)
				{
					Game->SwitchMode(GM_OPTIONSMENU);	// Go to Options Menu
				}
				else
				{
					WM->Quit();							// End main loop
				}
			}
			else
			{
				if(selected==0)
				{
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->StartNewGame();			// Back to Game
				}
				else if(selected==1)
				{
					Game->SwitchMode(GM_OPTIONSMENU);	// Go to Options Menu
				}
				else
				{
					WM->Quit();							// End main loop
				}
			}
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

void MenuController::ProcessOptionsMenu(SDL_Event *event)
{
	MainMenu *menu = (MainMenu *)SM->GetSceneByName("Main Menu");
	int selected = menu->GetSelection();
	int mode = menu->GetModeIndex();

	switch(event->type)
	{
	case SDL_KEYDOWN:
		switch(event->key.keysym.sym)
        {
		case SDLK_ESCAPE:
			Game->SwitchMode(GM_PLAY);
			break;
		case 'm':
			if(WM->IsMouseGrabbed())
				WM->ReleaseMouse();
			else
				WM->GrabMouse();
			break;
		case SDLK_DOWN:
			menu->SelectNext();
			break;
		case SDLK_UP:
			menu->SelectPrev();
			break;
		case SDLK_RIGHT:
			if(selected==1)
			{
				menu->SelectNextMode();
			}
			break;
		case SDLK_LEFT:
			if(selected==1)
			{
				menu->SelectPrevMode();
			}
			break;
		case SDLK_RETURN:
			if(selected==0)
			{
				WM->ToggleFullscreen();
			}
			else if(selected==1)
			{
				SDL_Rect **tmp = WM->GetAvailableModes();
				WM->SetFullscreenMode(tmp[mode]);
			}
			else
			{
				Game->SwitchMode(GM_MAINMENU);
			}
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
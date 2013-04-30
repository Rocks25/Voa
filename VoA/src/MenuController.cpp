#include "..\include\MenuController.h"
#include "..\include\Game.h"
#include "..\include\WindowManager.h"
#include "..\include\MainMenu.h"
#include "..\include\OptionsMenu.h"
#include "..\include\SoundManager.h"


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
					Sounds->StartSound("MenuStartGame");
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->SwitchMode(GM_PLAY);			// Back to Game
				}
				else if(selected==1)
				{
					Sounds->StartSound("MenuStartGame");
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->StartNewGame();				// Start a new Game
				}
				else if(selected==2)
				{
					Sounds->StartSound("MenuExecute");
					Game->SwitchMode(GM_OPTIONSMENU);	// Go to Options Menu
					menu->SetSelection(0);
				}
				else
				{
					Sounds->StartSound("MenuExecute");
					WM->Quit();							// End main loop
				}
			}
			else
			{
				if(selected==0)
				{
					Sounds->StartSound("MenuStartGame");
					SM->RemoveElement("Main Menu");
					SM->RemoveElement("Options Menu");
					Game->StartNewGame();			// Back to Game
				}
				else if(selected==1)
				{
					Sounds->StartSound("MenuExecute");
					Game->SwitchMode(GM_OPTIONSMENU);	// Go to Options Menu
					menu->SetSelection(0);
				}
				else
				{
					Sounds->StartSound("MenuQuit");
					Sounds->StopMusic();
					SDL_Delay(500);
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
			Game->SwitchMode(GM_MAINMENU);
			if(Game->HasGameStarted())
				menu->SetSelection(2);
			else
				menu->SetSelection(1);
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
			Sounds->StartSound("MenuExecute");
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
				if(Game->HasGameStarted())
					menu->SetSelection(2);
				else
					menu->SetSelection(1);
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
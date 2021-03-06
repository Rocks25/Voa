#include "..\include\GlobalEventManager.h"
#include "..\include\Game.h"

GlobalEventManager::GlobalEventManager(void)
{
}


GlobalEventManager::~GlobalEventManager(void)
{
}

void GlobalEventManager::ProcessEvent(SDL_Event *event)
{
	// If Keyboard event
	if(event->type==SDL_KEYDOWN || event->type==SDL_KEYUP || event->type == SDL_MOUSEMOTION)
	{
		//Send to Game class for it to send to proper handler
		Game->ProcessControlEvent(event);
	}
	if(event->type==SDL_VIDEORESIZE || SDL_VIDEOEXPOSE || SDL_ACTIVEEVENT)
	{
		WM->ProcessEvent(event);
	}
}
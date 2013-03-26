#include "..\include\GlobalEventManager.h"
#include "..\include\Game.h"

GlobalEventManager *GlobalEventManager::_instance = 0;

GlobalEventManager::GlobalEventManager(void)
{
}


GlobalEventManager::~GlobalEventManager(void)
{
	delete _instance;
}

GlobalEventManager *GlobalEventManager::GetSingleton()
{
	if(!_instance)
	{
		_instance = new GlobalEventManager();
	}
	return _instance;
}

void GlobalEventManager::ProcessEvent(SDL_Event *event)
{
	// If Keyboard event
	if(event->type==SDL_KEYDOWN || event->type==SDL_KEYUP)
	{
		//Send to Game class for it to send to proper handler
		Game::ProcessKeyboardEvent(event);
	}
}
#pragma once
#include <SDL.h>

class GlobalEventManager
{
private:
	bool _keys[323];

public:
	GlobalEventManager(void);
	~GlobalEventManager(void);
	void ProcessEvent(SDL_Event *event);
};

extern GlobalEventManager *GEM;
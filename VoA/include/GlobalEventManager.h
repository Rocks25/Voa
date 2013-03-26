#pragma once
#include <SDL.h>

class GlobalEventManager
{
private:
	static GlobalEventManager *_instance;
	bool _keys[323];

protected:
	GlobalEventManager(void);
	~GlobalEventManager(void);

public:
	static GlobalEventManager *GetSingleton();
	void ProcessEvent(SDL_Event *event);
};


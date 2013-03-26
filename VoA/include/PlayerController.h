#pragma once

#include <SDL.h>

class PlayerController
{
protected:
	PlayerController(void);
	~PlayerController(void);
public:
	void ProcessKeyboardEvent(SDL_Event *event);
};


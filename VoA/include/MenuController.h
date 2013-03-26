#pragma once

#include <SDL.h>

class MenuController
{
protected:
	MenuController(void);
	~MenuController(void);
public:
	static void ProcessKeyboardEvents(SDL_Event *event);
};


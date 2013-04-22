#pragma once

#include <SDL.h>

class MenuController
{
protected:
	MenuController(void);
	~MenuController(void);
public:
	static void ProcessKeyboardEvents(SDL_Event *event);
private:
	static void ProcessMainMenu(SDL_Event *event);
	static void ProcessOptionsMenu(SDL_Event *event);
};


#pragma once

#include <SDL.h>

class PlayerController
{
protected:
	bool keys[512];
public:
	PlayerController(void);
	~PlayerController(void);
	void ProcessKeyboardEvent(SDL_Event *event);
	void ProcessKeyboardControls();
};

extern PlayerController *PC;
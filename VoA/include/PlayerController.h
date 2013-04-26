#pragma once

#include <SDL.h>

class PlayerController
{
protected:
	bool keys[512];
	int mouse_x;
	int mouse_y;
public:
	PlayerController(void);
	~PlayerController(void);
	void ProcessControlEvent(SDL_Event *event);
	void ProcessControls();
	int GetMouseX();
	int GetMouseY();
};

extern PlayerController *PC;
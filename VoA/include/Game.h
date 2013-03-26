#pragma once

#define GM_MAINMENU			1
#define GM_OPTIONSMENU		2
#define GM_PLAY				3
#define GM_PAUSE			4
#define GM_QUIT				5
#include <SDL.h>

class Game
{
private:
	Game(void);
	~Game(void);
	static bool Running;

	static int _Mode;
public:
	static int GetCurrentMode();
	static void SwitchMode(int mode);
	static void ProcessKeyboardEvent(SDL_Event *event);
	static void Quit();
	static void Run();
	static bool IsRunning();
};


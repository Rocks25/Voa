#pragma once

#define GM_MAINMENU			1
#define GM_OPTIONSMENU		2
#define GM_PLAY				3
#define GM_PAUSE			4
#define GM_QUIT				5
#include <SDL.h>
#include "TextureManager.h"
#include "GlobalEventManager.h"
#include "WindowManager.h"
#include "SceneManager.h"
#include "OpenGLRenderer.h"
#include "MenuController.h"
#include "PlayerController.h"
#include "GlobalMatrices.h"

class Game
{
private:
	static bool _Running;
	static bool _Active;

	static int _Mode;
public:
	Game(void);
	~Game(void);
	static int GetCurrentMode();
	static void SwitchMode(int mode);
	static void ProcessKeyboardEvent(SDL_Event *event);
	static void Quit();
	static void Run();
	static void Render();
	static bool IsRunning();
	static bool IsActive();
	static void SetActiveState(bool state);
};


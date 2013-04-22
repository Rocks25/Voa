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
#include "Error.h"
#include "GlobalMatrices.h"

class GameManager
{
private:
	bool _Running;
	int _Mode;
	bool _GameStatus;
public:
	GameManager(void);
	~GameManager(void);
	int GetCurrentMode();
	void SwitchMode(int mode);
	void ProcessKeyboardEvent(SDL_Event *event);
	void Run();
	void Render();
	void Pause();
	void StartNewGame();
	bool HasGameStarted();
};

extern GameManager *Game;
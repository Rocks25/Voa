#include "..\include\Game.h"

bool Game::_Running = false;
bool Game::_Active = true;
int Game::_Mode = GM_MAINMENU;


TextureManager *TM;
GlobalEventManager *GEM;
WindowManager *WM;
SceneManager *SM;
GlobalMatrices *GMat;
PlayerController *PC;

Game::Game(void)
{
	TM = new TextureManager();
	GEM = new GlobalEventManager();
	WM = new WindowManager();
	SM = new SceneManager();
	GMat = new GlobalMatrices();
	PC = new PlayerController();
	//MC = new MenuController();
}


Game::~Game(void)
{
	delete TM;
	delete GEM;
	delete WM;
	delete SM;
	delete GMat;
	delete PC;
}

int Game::GetCurrentMode()
{
	return _Mode;
}

void Game::SwitchMode(int mode)
{
	_Mode=mode;
}

void Game::ProcessKeyboardEvent(SDL_Event *event)
{
	if(_Mode == GM_MAINMENU)
		MenuController::ProcessKeyboardEvents(event);
	else if(_Mode == GM_PLAY)
		PC->ProcessKeyboardEvent(event);
}

void Game::Quit()
{
	_Running=false;
}

void Game::Run()
{
	_Running=true;
}

bool Game::IsRunning()
{
	return _Running;
}

bool Game::IsActive()
{
	return _Active;
}

void Game::SetActiveState(bool state)
{
	_Active = state;
}
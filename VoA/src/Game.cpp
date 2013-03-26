#include "..\include\Game.h"
#include "..\include\PlayerController.h"
#include "..\include\MenuController.h"

bool Game::Running = false;
int Game::_Mode = GM_MAINMENU;

Game::Game(void)
{
}


Game::~Game(void)
{
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
	MenuController::ProcessKeyboardEvents(event);
}

void Game::Quit()
{
	Running=false;
}

void Game::Run()
{
	Running=true;
}

bool Game::IsRunning()
{
	return Running;
}
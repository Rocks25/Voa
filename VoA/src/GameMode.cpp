#include "..\include\GameMode.h"

GameMode *GameMode::_instance = 0;

GameMode::GameMode(void)
{
	_Mode=GM_PLAY;
}


GameMode::~GameMode(void)
{
	delete _instance;
}

GameMode *GameMode::GetSingleton()
{
	if(!_instance)
	{
		_instance = new GameMode;
	}
	return _instance;
}

int GameMode::GetCurrentMode()
{
	return _Mode;
}

void GameMode::SwitchMode(int mode)
{
	_Mode=mode;
}
#pragma once

#define GM_MAINMENU			1
#define GM_OPTIONSMENU		2
#define GM_PLAY				3
#define GM_PAUSE			4
#define GM_QUIT				5

class GameMode
{
private:
	GameMode(void);
	~GameMode(void);
	static GameMode *_instance;

	int _Mode;
public:
	static GameMode *GetSingleton();
	int GetCurrentMode();
	void SwitchMode(int mode);
};


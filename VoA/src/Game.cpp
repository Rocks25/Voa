#include "..\include\Game.h"
#include "..\include\Plane.h"
#include "..\include\ship.h"
#include "..\include\MainMenu.h"
#include "..\include\HUD.h"
#include "..\include\OptionsMenu.h"
#include "..\include\AsteroidSystem.h"
#include "..\include\SoundManager.h"
#include <time.h>

TextureManager *TM;
GlobalEventManager *GEM;
WindowManager *WM;
SceneManager *SM;
GlobalMatrices *GMat;
PlayerController *PC;
Errors *Error;
SoundManager *Sounds;
//AsteroidSystem *AS;

GameManager::GameManager(void)
{
	srand(time(NULL));
	TM = new TextureManager();
	GEM = new GlobalEventManager();
	WM = new WindowManager();
	SM = new SceneManager();
	GMat = new GlobalMatrices();
	PC = new PlayerController();
	Sounds = new SoundManager();
	//MC = new MenuController();
	Error = new Errors();
	_GameStatus = false;
	_Running = false;
	_Mode = GM_MAINMENU;
	_DebugMode = false;
}


GameManager::~GameManager(void)
{
	delete TM;
	delete GEM;
	delete WM;
	delete SM;
	delete GMat;
	delete PC;
	delete Error;
}

void GameManager::InitMusic()
{
	//Music
	Sounds->AddMusic("A Flame of the Hear","Audio/Music/A-Flame-of-the-Hear.ogg");
	Sounds->AddMusic("Cosmic Gate -- Exploration Of Space","Audio/Music/Cosmic Gate -- Exploration Of Space.ogg");
	Sounds->AddMusic("Dnor vs Ull Never G","Audio/Music/Dnor-vs-Ull-Never-G.ogg");
	Sounds->AddMusic("GEN eRiK","Audio/Music/GEN eRiK.ogg");
	Sounds->AddMusic("Pre Valent","Audio/Music/Pre Valent.ogg");
	Sounds->AddMusic("Groove Grove","./Audio/Music/Groove_Grove.ogg");
	Sounds->AddMusic("Yoji Biomehanika - Hardstyle Disco","./Audio/Music/Yoji Biomehanika - Hardstyle Disco.ogg");

	//Menu Sound Effects
	Sounds->AddSound("MenuClick", "./Audio/Effects/MenuClick.ogg");
	Sounds->AddSound("MenuExecute", "./Audio/Effects/MenuExecute.ogg");
	Sounds->AddSound("MenuQuit", "./Audio/Effects/MenuQuit.ogg");
	Sounds->AddSound("MenuStartGame", "./Audio/Effects/MenuStartGame.ogg");

	//Start Music
	Sounds->PlayRandomSong();
}

int GameManager::GetCurrentMode()
{
	return _Mode;
}

void GameManager::SwitchMode(int mode)
{
	_Mode=mode;
	if(_Mode == GM_PLAY)
	{
		_LastTime = SDL_GetTicks();
	}
}

void GameManager::ProcessControlEvent(SDL_Event *event)
{
	if(event->type==SDL_KEYDOWN && event->key.keysym.sym == '.')
	{
		Sounds->PlayRandomSong();
	}
	if(_Mode == GM_MAINMENU || _Mode == GM_OPTIONSMENU)
		MenuController::ProcessKeyboardEvents(event);
	else if(_Mode == GM_PLAY)
		PC->ProcessControlEvent(event);
}

void GameManager::InitResources()
{
	InitMusic();
}

bool GameManager::HasGameStarted()
{
	return _GameStatus;
}

void GameManager::StartNewGame()
{
	_LastTime = SDL_GetTicks();
	if(_GameStatus)
	{
		if(SM->GetSceneByName("Main"))
		{
			SM->GetSceneByName("Main")->Cleanup();
		}
	}
	Entity *ent = new Entity("Player");
	Plane *dummydir = new Plane("ShipDummy",16,16);
	HUD *hud = new HUD();
	Ship *ship = new Ship(1.0f,"Player Ship");
	float width = ship->GetBoundingBox().x;
	ship->Init();

	TM->AddTexture("images/ship/fire.jpg","Ship Flame");
	TM->AddTexture("images/ship/fire_alpha.jpg", "Ship Flame Alpha");

	Plane *le = new Plane("LEngine",64,64, "Ship Flame", "Ship Flame Alpha");
	Plane *re = new Plane("REngine",64,64, "Ship Flame", "Ship Flame Alpha");
	le->Init();
	re->Init();
	le->SetScale(glm::vec3(0,0,0));
	re->SetScale(glm::vec3(0,0,0));

	le->SetPosition(glm::vec3(WM->GetWindowWidth()/2.0f-16,WM->GetWindowHeight()/2.0f-32,0));

	//lEngine->SetPositionMat(glm::translate(glm::mat4(1.0f),glm::vec3(-newWidth,-10.0,0.0)));
	/*TM->AddTexture("images/particle1.jpg","Particle1");
	TM->AddTexture("images/particle1_alpha.jpg","Particle1_Alpha");*/

	//ship->SetPosition(glm::vec3(0,0,0));
	//ship->SetPosition(glm::vec3(WM->GetWindowWidth()/2.0f,WM->GetWindowHeight()/2.0f,0));
	//ship->SetRotation(glm::vec3(0,0,0));
	ship->SetScale(glm::vec3(1.0,1.0,1.0));

	dummydir->Translate(ship->GetPosition()+glm::vec3(1000,0,0));

	ent->AddMesh(le);
	ent->AddMesh(re);
	ent->AddMesh(ship);
	ent->AddMesh(dummydir);
	ent->AddMesh(hud);


	ent->SetPosition(0,0,0);

	AsteroidSystem *AS = new AsteroidSystem();
	AS->Init();

	if(!SM->GetSceneByName("Main"))
		SM->NewHUDElement("Main");
	SM->GetSceneByName("Main")->AddEntity(AS);
	SM->GetSceneByName("Main")->AddEntity(ent);
	SM->GetSceneByName("Main")->Init();
	_GameStatus = true;
	_Mode = GM_PLAY;

	Sounds->AddSound("Explosion", "Audio/Effects/explosion.ogg");
	Sounds->AddSound("Fire", "Audio/Effects/fire.ogg");
	Sounds->AddSound("Thrusters", "Audio/Effects/Thrusters.ogg");

	_TimeElapsed = 0;
}

void GameManager::Pause()
{
	if(_LastTime)
	{
		_TimeElapsed += SDL_GetTicks() - _LastTime;
	}

	// Create Menu

	MainMenu *mainmenu = new MainMenu("Main Menu");
	SM->AddElement(mainmenu);
	SM->GetCurrentScene()->Init();
	
	OptionsMenu *optionsmenu = new OptionsMenu("Options Menu");
	SM->AddElement(optionsmenu);
	SM->GetCurrentScene()->Init();

	Game->SwitchMode(GM_MAINMENU);
}

void GameManager::ToggleDebugMode()
{
	_DebugMode = !_DebugMode;
}

bool GameManager::IsDebugMode()
{
	return _DebugMode;
}

Uint32 GameManager::GetTimeElapsed()
{
	Uint32 Time = _TimeElapsed;
	if(_Mode == GM_PLAY)
	{
		Time += SDL_GetTicks() -_LastTime;
	}
	return Time;
}
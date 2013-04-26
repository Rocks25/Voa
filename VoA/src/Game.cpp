#include "..\include\Game.h"
#include "..\include\Plane.h"
#include "..\include\ship.h"
#include "..\include\MainMenu.h"
#include "..\include\OptionsMenu.h"

TextureManager *TM;
GlobalEventManager *GEM;
WindowManager *WM;
SceneManager *SM;
GlobalMatrices *GMat;
PlayerController *PC;
Errors *Error;

GameManager::GameManager(void)
{
	TM = new TextureManager();
	GEM = new GlobalEventManager();
	WM = new WindowManager();
	SM = new SceneManager();
	GMat = new GlobalMatrices();
	PC = new PlayerController();
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

int GameManager::GetCurrentMode()
{
	return _Mode;
}

void GameManager::SwitchMode(int mode)
{
	_Mode=mode;
}

void GameManager::ProcessControlEvent(SDL_Event *event)
{
	if(_Mode == GM_MAINMENU || _Mode == GM_OPTIONSMENU)
		MenuController::ProcessKeyboardEvents(event);
	else if(_Mode == GM_PLAY)
		PC->ProcessControlEvent(event);
}

void GameManager::Run()
{
	_Running=true;
}

bool GameManager::HasGameStarted()
{
	return _GameStatus;
}

void GameManager::StartNewGame()
{
	if(_GameStatus)
	{
		if(SM->GetSceneByName("Main") != 0)
		{
			SM->RemoveElement("Main");
		}
	}
	Entity *ent = new Entity("Player");
	Plane *dummydir = new Plane("ShipDummy",16,16);
	Plane *HUD = new Plane("HUD",WM->GetWindowWidth(),WM->GetWindowHeight(),"HUD","HUD Alpha");
	Ship *ship = new Ship(1.0f,"Player Ship");
	float size = ship->GetSize();
	float width = ship->GetBoundingBox().x;
	float newWidth = width*size;
	ship->Init();

	TM->AddTexture("images/ship/fire.jpg","Ship Flame");
	TM->AddTexture("images/ship/fire_alpha.jpg", "Ship Flame Alpha");

	Plane *le = new Plane("LEngine",128,128, "Ship Flame", "Ship Flame Alpha");
	Plane *re = new Plane("REngine",128,128, "Ship Flame", "Ship Flame Alpha");
	le->Init();
	re->Init();

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
	ent->AddMesh(HUD);


	ent->SetPosition(0,0,0);

	SM->NewHUDElement("Main");
	SM->GetCurrentScene()->AddEntity(ent);
	SM->GetCurrentScene()->Init();
	_GameStatus = true;
	_Mode = GM_PLAY;

}

void GameManager::Pause()
{
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
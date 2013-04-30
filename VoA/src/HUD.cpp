#include "../include/HUD.h"
#include "../include/Game.h"
#include "../include/Font.h"
#include "../include/Plane.h"


HUD::HUD(void) : Mesh("HUD")
{
}


HUD::~HUD(void)
{
}

void HUD::Render()
{
	Uint32 elapsed = Game->GetTimeElapsed();
	int minutes = elapsed/1000.0f/60.0f;
	int seconds = elapsed/1000.0f-minutes*60.0f;
	int mseconds = elapsed-seconds*1000.0f;
	char buf[255] = {0};
	sprintf_s(buf, "%dm %ds",minutes,seconds);
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("HUD");
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("HUD Alpha");

	GMat->ModelMatrix()->PushMatrix();
	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	Plane::Render(0,0,WM->GetWindowWidth(),WM->GetWindowHeight());
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(650.0f/800.0f*width,700.0f/800.0f*height,0.0f);
	GMat->UpdateShader();
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("White");
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	Font::Render("Time Elapsed:","Text/simplicity",Color(255,255,255,255),24);
	GMat->ModelMatrix()->Translate(0.0f,20.0f,0.0f);
	GMat->UpdateShader();
	Font::Render(buf,"Text/simplicity",Color(255,255,255,255),24);
	GMat->ModelMatrix()->PopMatrix();
	GMat->UpdateShader();
}
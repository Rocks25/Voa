#include "..\include\Error.h"
#include "..\include\Font.h"
#include "..\include\TextureManager.h"
#include "..\include\GlobalMatrices.h"
#include "..\include\WindowManager.h"


Error::Error(void)
{
}


Error::~Error(void)
{
}

void Error::Print(const char *text)
{
	TM->BindTexture("White");
	GMat->ModelMatrix()->PushMatrix();
	GMat->ModelMatrix()->Translate(WM->GetWindowWidth()/2.0f,0.0f,0.0f);
	GMat->UpdateShader();
	Font::Render(text,"fonts/arial rounded.ttf", Color(255,0,0,160),12);
	GMat->ModelMatrix()->PopMatrix();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}
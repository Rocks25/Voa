#include "..\include\Error.h"
#include "..\include\Font.h"
#include "..\include\TextureManager.h"
#include "..\include\GlobalMatrices.h"
#include "..\include\WindowManager.h"


Errors::Errors(void)
{
}


Errors::~Errors(void)
{
	for(int i=0; i<messages.size();i++)
	{
		delete messages[i].message;
	}
}

void Errors::NewError(char *text)
{
	ErrorMessage msg;
	msg.message = new char[512];
	memset(msg.message,0,512);
	strcpy(msg.message,text);
	msg.life = 180;
	messages.push_back(msg);
}

void Errors::PrintErrors()
{
	if(messages.size()<=0)
		return;
	TM->BindTexture("White");
	GMat->ModelMatrix()->PushMatrix();
	GMat->ModelMatrix()->Translate(WM->GetWindowWidth()/2.0f,0.0f,0.0f);
	GMat->UpdateShader();
	//for(unsigned int i=messages.size()-1;i>messages.size()-6;i--)
	for(unsigned int i=0; i < messages.size(); i++)
	{
		messages[i].life--;
		if(messages[i].life<=0)
		{
			delete messages[i].message;
			messages.erase(messages.begin()+i);
			continue;
		}
		if(messages.size() > 5)
		{
			if(i >= messages.size()-5)
			{
				GMat->ModelMatrix()->Translate(0,24,0);
				GMat->UpdateShader();
				Font::Render(messages[i].message,"arial rounded", Color(255,0,0,160),12);
			}
		}
		else
		{
			GMat->ModelMatrix()->Translate(0,24,0);
			GMat->UpdateShader();
			Font::Render(messages[i].message,"arial rounded", Color(255,0,0,160),12);
		}
	}
	GMat->ModelMatrix()->PopMatrix();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}
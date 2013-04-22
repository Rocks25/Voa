#include "../include/OptionsMenu.h"
#include "../include/Font.h"
#include "../include/TextureManager.h"
#include "../include/WindowManager.h"

OptionsMenu::OptionsMenu(char *name) : Scene(name)
{
	_Name = name;
	a1inc=a2inc=true;
	_Initialized=false;
	selection=0;
	modeselected=0;
}

OptionsMenu::~OptionsMenu(void)
{
	Cleanup();
}

void OptionsMenu::Cleanup()
{
	if(!objects.empty())
	{
		objects.clear();
	}
}

void OptionsMenu::Init()
{
	_Initialized=true;
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	ScreenCenter = glm::vec2(width/2.0f, height/2.0f);

	Plane *p1 = new Plane("Asteroid1",320/800.0f*width, 240/600.0f*height,"Asteroid1");
	Plane *p2 = new Plane("Asteroid2",320/800.0f*width, 240/600.0f*height,"Asteroid2");
	Plane *p3 = new Plane("Background",(float)width, (float)height,"Background");
	objects.push_back(p1);
	objects.push_back(p2);
	objects.push_back(p3);

	pos1 = glm::translate(glm::vec3(-100.0f,-75.0f,0.0f));
	pos2 = glm::translate(glm::vec3( 210.0f, 60.0f,0.0f));
	pos3 = glm::translate(glm::vec3(   0.0f,  0.0f,0.0f));
	
	dir1 = glm::vec3((rand()%1000/1000.0f)*4-.2,(rand()%1000/1000.0f)*4-.2,0.0f);
	dir2 = glm::vec3((rand()%1000/1000.0f)*4-.2,(rand()%1000/1000.0f)*4-.2,0.0f);

	alpha1=alpha2=0.1f;

	arate1=rand()%100/10000.0f*3;
	arate2=rand()%100/10000.0f*3;
	a1rot=rand()%100/100.0f*180;
	a2rot=rand()%100/100.0f*180;
	InitTextures();
}

void OptionsMenu::Render()
{
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	WM->GetRenderer()->BindShaderProgram("Main");
	GLShaderProgram *program = WM->GetRenderer()->GetCurrentShader();
	if(!IsInitialized())
	{
		Error->NewError("OptionsMenu class was not initialized");
		return;
	}

	
	glm::vec4 pos = pos1*glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
	
	glColor4f(0.0f,0.0f,0.0f,alpha1);
	if(alpha1+arate1<=0)
	{
		alpha1=0.0f;
		a1inc=true;
		arate1=rand()%10/1000.0f+.005f;
		float x = rand()%1000/1000.0f*width-width/ScreenCenter.x;
		float y = rand()%1000/1000.0f*height-height/ScreenCenter.y;
		float xCenter = ScreenCenter.x/2.0f;
		float yCenter = ScreenCenter.y/2.0f;
		float randx = rand()%1000/1000.0f;
		float randy = rand()%1000/1000.0f;
		float dirx = abs(x-xCenter)/(xCenter-x)*randx*4;
		float diry = abs(y-yCenter)/(yCenter-y)*randy*4;
		dir1.x=dirx;
		dir1.y=diry;
		a1rot=rand()%100/100.0f*180;
		pos1 = glm::rotate(a1rot,glm::vec3(0,0,1))+glm::translate(glm::vec3(x,y,0.0f));
		alpha1+=arate1;
	}
	else if(alpha1+arate1>=1.0f)
	{
		a1inc=false;
		arate1=-arate1;
		alpha1+=arate1;
		pos1 = glm::translate(pos1,glm::vec3(dir1.x,dir1.y,0.0f));
	}
	else
	{
		alpha1+=arate1;
		pos1 = glm::translate(pos1,glm::vec3(dir1.x,dir1.y,0.0f));
	}
	glColor4f(1.0f,1.0f,1.0f,alpha1);
	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(pos1*glm::vec4(1.0f,1.0f,1.0f,1.0f)));
	GMat->UpdateShader();
	objects[0]->Render();
	GMat->ModelMatrix()->LoadIdentity();
	
	glColor4f(0.0f,0.0f,0.0f,alpha2);
	if(alpha2+arate2<=0)
	{
		alpha2=0.0f;
		a2inc=true;
		arate2=rand()%10/1000.0f+.005f;
		float x = rand()%1000/1000.0f*width-width/ScreenCenter.x;
		float y = rand()%1000/1000.0f*height-height/ScreenCenter.x;
		float xCenter = ScreenCenter.x;
		float yCenter = ScreenCenter.y;
		float randx = rand()%1000/1000.0f;
		float randy = rand()%1000/1000.0f;
		float dirx = abs(x-xCenter)/(xCenter-x)*randx*4;
		float diry = abs(y-yCenter)/(yCenter-y)*randy*4;
		dir2.x=dirx;
		dir2.y=diry;
		a2rot=rand()%100/100.0f*180;
		pos2 = glm::rotate(a2rot,glm::vec3(0,0,1))+glm::translate(glm::vec3(x,y,0.0f));
		alpha2+=arate2;
	}
	else if(alpha2+arate2>=1.0f)
	{
		a2inc=false;
		arate2=-arate2;
		alpha2+=arate2;
		pos2 = glm::translate(pos2,glm::vec3(dir2.x,dir2.y,0.0f));
	}
	else
	{
		alpha2+=arate2;
		pos2 = glm::translate(pos2,glm::vec3(dir2.x,dir2.y,0.0f));
	}
	glColor4f(1.0f,1.0f,1.0f,alpha2);
	
	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(pos2*glm::vec4(1.0f,1.0f,1.0f,1.0f)));
	GMat->UpdateShader();
	objects[1]->Render();

	glColor4f(0.0f,0.0f,0.0f,1.0f);
	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	objects[2]->Render();

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*300,height/1050.0f*100,0));
	GMat->UpdateShader();
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glActiveTexture(GL_TEXTURE0);
    TM->BindTexture("GreenHD");
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("White");

	Color col(255,255,255);
	int size = (int)(48/800.0f*height);
	char *font = "Text/simplicity";
	Font::Render("Variations on Asteroids","Title/Dodgv2ci",col,size);

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*100,height/1050.0f*525,0));
	GMat->UpdateShader();
	char buf[255] = {0};
	if(WM->IsFullscreen())
		strcpy(buf, "Fullscreen: True");
	else
		strcpy(buf, "Fullscreen: False");
	Font::Render(buf, font, col,size);

	if(!WM->IsFullscreen())
	{
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("White");
	}
	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*100,height/1050.0f*625,0));
	GMat->UpdateShader();
	memset(buf,0,32);
	SDL_Rect *tmp = WM->GetAvailableModes()[modeselected];
	sprintf(buf,"%d x %d",tmp->w,tmp->h);
	Font::Render(buf, font, col,size);
	if(!WM->IsFullscreen())
	{
		glActiveTexture(GL_TEXTURE0);
		TM->BindTexture("GreenHD");
	}

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*100,height/1050.0f*725,0));
	GMat->UpdateShader();
	Font::Render("Back", font, col,size);
	
	glColor4f(0.0f,0.0f,0.0f,0.5f);
	glActiveTexture(GL_TEXTURE0);
    TM->BindTexture("Background");
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("White");

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*100,height/1050.0f*(525+100*selection),0));
	GMat->UpdateShader();
	Plane::Render(0,0,150/600.0f*width,55/800.0f*height);
	
	glColor4f(0.0f,0.0f,0.0f,1.0f);
}

void OptionsMenu::Resize()
{
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	ScreenCenter = glm::vec2(width/2.0f,height/2.0f);
	/*for(unsigned int i=0;i<objects.size();i++)
	{
		delete objects[i];
	}*/
	for(unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
	Plane *p1 = new Plane("Rock1",320/800.0f*width, 240/600.0f*height,"Rock1", "Rock1 Alpha");
	objects.push_back(p1);
	Plane *p2 = new Plane("Rock2",320/800.0f*width, 240/600.0f*height,"Rock2", "Rock2 Alpha");
	objects.push_back(p2);
	Plane *p3 = new Plane("Background",(float)width,(float)height,"Background");
	objects.push_back(p3);
}

void OptionsMenu::InitTextures()
{
	TM->AddTexture("images/grey.jpg","Grey");
}

bool OptionsMenu::IsInitialized()
{
	return _Initialized;
}

void OptionsMenu::SelectNext()
{
	selection++;
	if(selection > 2)
		selection = 0;
	if(selection == 1 && !WM->IsFullscreen())
		selection++;
}

void OptionsMenu::SelectPrev()
{
	selection--;
	if(selection < 0)
		selection = 2;
	if(selection == 1 && !WM->IsFullscreen())
		selection--;
}

void OptionsMenu::SelectNextMode()
{
	modeselected++;
	if(modeselected >= WM->GetNumAvailableModes())
		modeselected=0;
}

void OptionsMenu::SelectPrevMode()
{
	modeselected--;
	if(modeselected < 0)
		modeselected=WM->GetNumAvailableModes()-1;
}

int OptionsMenu::GetSelection()
{
	return selection;
}

int OptionsMenu::GetModeIndex()
{
	return modeselected;
}
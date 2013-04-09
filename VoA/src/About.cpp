#include "../include/About.h"
#include "../include/Font.h"
#include "../include/TextureManager.h"
#include "../include/WindowManager.h"

About::About(char *name) : Scene(name)
{
	_Name = name;
	a1inc=a2inc=true;
	_Initialized=false;
}

About::~About(void)
{
	Cleanup();
}

void About::Cleanup()
{
	if(!objects.empty())
	{
		objects.clear();
	}
}

void About::Init()
{
	_Initialized=true;
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	ScreenCenter = glm::vec2(width/2.0f, height/2.0f);

	Plane *p1 = new Plane(320/800.0f*width, 240/600.0f*height);
	Plane *p2 = new Plane(320/800.0f*width, 240/600.0f*height);
	Plane *p3 = new Plane((float)width, (float)height);
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

void About::Render()
{
	int width = WM->GetWindowWidth();
	int height = WM->GetWindowHeight();
	WM->GetRenderer()->BindShaderProgram("Main");
	GLShaderProgram *program = WM->GetRenderer()->GetCurrentShader();
	if(!IsInitialized())
	{
		MessageBox(NULL,"About class was not initialized","ERROR",MB_OK);
		return;
	}

	
	glm::vec4 pos = pos1*glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("White");
    program->SetUniformValue("alpha",1);
	
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
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("Asteroid2");
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("White");
    program->SetUniformValue("alpha",1);
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
	glActiveTexture(GL_TEXTURE0);
    TM->BindTexture("Asteroid1");
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("White");
    program->SetUniformValue("alpha",1);
	objects[1]->Render();

	glColor4f(0.0f,0.0f,0.0f,1.0f);
	GMat->ModelMatrix()->LoadIdentity();
	GMat->UpdateShader();
	glActiveTexture(GL_TEXTURE0);
    TM->BindTexture("Asteroids");
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("White");
    program->SetUniformValue("alpha",1);
	objects[2]->Render();

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*300,height/1050.0f*100,0));
	GMat->UpdateShader();
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glActiveTexture(GL_TEXTURE0);
    TM->BindTexture("GreenHD");
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    TM->BindTexture("Partice1Alpha");
    program->SetUniformValue("alpha",1);

	Color col(255,255,255);
	int size = (int)(48/800.0f*height);
	char *font = "fonts/arial rounded.TTF";
	Font::Render("Variations on Asteroids",font,col,size);

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*1000,height/1050.0f*525,0));
	GMat->UpdateShader();
	Font::Render("F1 - Play Game", font, col,size);

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*1000,height/1050.0f*625,0));
	GMat->UpdateShader();
	Font::Render("F2 - Options", font, col,size);

	GMat->ModelMatrix()->LoadIdentity();
	GMat->ModelMatrix()->Translate(glm::vec3(width/1680.0f*1000,height/1050.0f*725,0));
	GMat->UpdateShader();
	Font::Render("Q - Quit Game", font, col,size);
}

void About::Resize()
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
	Plane *p1 = new Plane(320/800.0f*width, 240/600.0f*height);
	objects.push_back(p1);
	Plane *p2 = new Plane(320/800.0f*width, 240/600.0f*height);
	objects.push_back(p2);
	Plane *p3 = new Plane((float)width,(float)height);
	objects.push_back(p3);
}

void About::InitTextures()
{
    TM->AddTexture("images/about/asteroid2.jpg", "Asteroid2");
	TM->AddTexture("images/about/asteroid1.jpg", "Asteroid1");
	TM->AddTexture("images/about/asteroids.jpg", "Asteroids");
	TM->AddTexture("images/particle2_alpha.jpg", "Particle2Alpha");
	TM->AddTexture("images/particle1_alpha.jpg", "Particle1Alpha");
	TM->AddTexture("images/about/green-hd-14.jpg", "GreenHD");
}

bool About::IsInitialized()
{
	return _Initialized;
}
#include "../include/About.h"
#include "../include/Plane.h"
#include "../include/Font.h"

About::About()
{
	a1inc=a2inc=true;
	ScreenCenter = glm::vec2(_width/2.0f, _height/2.0f);
}

About::~About(void)
{
	for(unsigned int i=0; i<objects.size(); i++)
	{
		delete objects[i];
	}
	objects.clear();
}

void About::Init(SDL_Surface *window)
{
	_width = window->w;
	_height = window->h;

	Plane *p1 = new Plane(320/800.0f*_width, 240/600.0f*_height);
	Plane *p2 = new Plane(320/800.0f*_width, 240/600.0f*_height);
	Plane *p3 = new Plane((float)_width, (float)_height);
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

void About::Render(GLShaderProgram *program)
{
	int modelMatLoc = glGetUniformLocation(program->GetProgramID(),"modelMat");

	
	glm::vec4 pos = pos1*glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    program->SetUniformValue("alpha",1);
	
	glColor4f(1.0f,1.0f,1.0f,alpha1);
	if(alpha1+arate1<=0)
	{
		alpha1=0.0f;
		a1inc=true;
		arate1=rand()%10/1000.0f+.005f;
		float x = rand()%1000/1000.0f*_width-_width/ScreenCenter.x;
		float y = rand()%1000/1000.0f*_height-_height/ScreenCenter.y;
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
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&pos1[0][0]);					// Send Model Matrix to Shader
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    program->SetUniformValue("alpha",1);
	objects[0]->Render();
	
	glColor4f(1.0f,1.0f,1.0f,alpha2);
	if(alpha2+arate2<=0)
	{
		alpha2=0.0f;
		a2inc=true;
		arate2=rand()%10/1000.0f+.005f;
		float x = rand()%1000/1000.0f*_width-_width/ScreenCenter.x;
		float y = rand()%1000/1000.0f*_height-_height/ScreenCenter.x;
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
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&pos2[0][0]);					// Send Model Matrix to Shader
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    program->SetUniformValue("tex",0);
	objects[1]->Render();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&pos3[0][0]);					// Send Model Matrix to Shader
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    program->SetUniformValue("tex",0);
	objects[2]->Render();

	TTF_Font *fon = TTF_OpenFont("fonts/arial rounded.TTF",(int)(48/800.0f*_height));
	if(!fon)
	{
		char buf[512];
		sprintf_s(buf,"Error loading font: %s", TTF_GetError());
		MessageBox(NULL,buf,"Font Error!",MB_OK);
	}
	SDL_Color col;
	col.r = col.g = col.b = 255;
	glm::mat4 mat = glm::translate(glm::vec3(_width/1680.0f*300,_height/1050.0f*100,0));
	//glm::mat4 mat = glm::translate(glm::vec3(0.0f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    program->SetUniformValue("alpha",1);
	Font::Render("Variations on Asteroids",fon,col);
	mat = glm::translate(glm::vec3(_width/1680.0f*1000,_height/1050.0f*525,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
	Font::Render("Play Game", fon, col);
	mat = glm::translate(glm::vec3(_width/1680.0f*1000,_height/1050.0f*625,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
	Font::Render("Options", fon, col);
	mat = glm::translate(glm::vec3(_width/1680.0f*1000,_height/1050.0f*725,0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,&mat[0][0]);					// Send Model Matrix to Shader
	Font::Render("Quit Game", fon, col);
}

void About::Resize(int width, int height)
{
	_width = width;
	_height = height;
	ScreenCenter = glm::vec2(_width/2.0f,_height/2.0f);
	for(unsigned int i=0;i<objects.size();i++)
	{
		delete objects[i];
	}
	objects.clear();
	Plane *p1 = new Plane(320/800.0f*_width, 240/600.0f*_height);
	objects.push_back(p1);
	Plane *p2 = new Plane(320/800.0f*_width, 240/600.0f*_height);
	objects.push_back(p2);
	Plane *p3 = new Plane((float)_width,(float)_height);
	objects.push_back(p3);
}

void About::InitTextures()
{
	glGenTextures(6,texture);
    SDL_Surface *a = IMG_Load("images/about/asteroid2.jpg");
	SDL_PixelFormat *format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[0] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);

	a = IMG_Load("images/about/asteroid1.jpg");
	format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[1] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);

	
	a = IMG_Load("images/about/asteroids.jpg");
	format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[2] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);
	
	a = IMG_Load("images/particle2_alpha.jpg");
	format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[3] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);

	a = IMG_Load("images/particle1_alpha.jpg");
	format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[4] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);

	a = IMG_Load("images/about/green-hd-14.jpg");
	format = a->format;
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[5] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);
}
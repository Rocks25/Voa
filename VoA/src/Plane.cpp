#include "../include/Plane.h"
#include "../include/window_includes.h"
#include "../include/TextureManager.h"


Plane::Plane(char *name, float width,float height, char *difftex, char *alphatex) : Mesh(name)
{
	_Name = name;
	_width = width;
	_height = height;
	strncpy(_difftex, difftex, 254);
	strncpy(_alphatex, alphatex,254);
}


Plane::~Plane(void)
{
}

void Plane::Init()
{
	InitTextures();
	
}

void Plane::Render()
{
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture(_difftex);

	
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture(_alphatex);

	glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

	glTexCoord2d(1,1); glVertex3f(_width,_height,0.0f); // Top Right
	glTexCoord2d(0,1); glVertex3f(  0.0f,_height,0.0f); // Top Left
	glTexCoord2d(1,0); glVertex3f(_width,   0.0f,0.0f); // Bottom Right
	glTexCoord2d(0,0); glVertex3f(  0.0f,   0.0f,0.0f); // Bottom Left

	glEnd();                        // Done Building Triangle Strip
}

void Plane::Render(float x, float y, float width, float height)
{
	glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

	glTexCoord2d(1,1); glVertex3f(x+width,y+height,0.0f); // Top Right
	glTexCoord2d(0,1); glVertex3f( x+0.0f,y+height,0.0f); // Top Left
	glTexCoord2d(1,0); glVertex3f(x+width,  y+0.0f,0.0f); // Bottom Right
	glTexCoord2d(0,0); glVertex3f( x+0.0f,  y+0.0f,0.0f); // Bottom Left

	glEnd();                        // Done Building Triangle Strip
}

void Plane::RenderInverted(float x, float y, float width, float height)
{
	glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

	glTexCoord2d(1,0); glVertex3f(x+width,y+height,0.0f); // Top Right
	glTexCoord2d(0,0); glVertex3f( x+0.0f,y+height,0.0f); // Top Left
	glTexCoord2d(1,1); glVertex3f(x+width,  y+0.0f,0.0f); // Bottom Right
	glTexCoord2d(0,1); glVertex3f( x+0.0f,  y+0.0f,0.0f); // Bottom Left

	glEnd();                        // Done Building Triangle Strip
}

void Plane::SetDiffuseTexture(char *name)
{
	if(TM->TextureExists(name))
	{
		strncpy(_difftex,name,254);
	}
}

void Plane::SetAlphaTexture(char *name)
{
	if(TM->TextureExists(name))
	{
		strncpy(_alphatex,name,254);
	}
}
#include "../include/Plane.h"
#include "../include/WindowFunctions.h"


Plane::Plane(float width,float height)
{
	_width = width;
	_height = height;
}


Plane::~Plane(void)
{
}

void Plane::Init()
{
}

void Plane::Render()
{
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
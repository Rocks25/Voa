#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>
#include "GLShaderProgram.h"
#include "Color.h"

class Font
{
public:
	Font();
	~Font();
	static void Render(char *text,const char *font="arial rounded", Color color=Color(255,255,255,255),int ptsize=32);
private:
	static int NextPowerOfTwo(int x);
	static int Round(double x);
};

#endif//__FONT_H__
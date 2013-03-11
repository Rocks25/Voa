#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>
#include "GLShaderProgram.h"

class Font
{
public:
	Font();
	~Font();
	static void Render(const char *text,TTF_Font *font, SDL_Color color);
private:
	static int NextPowerOfTwo(int x);
	static int Round(double x);
};

#endif//__FONT_H__
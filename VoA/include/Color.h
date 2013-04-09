#pragma once

class Color
{
public:
	int r;
	int g;
	int b;
	int a;
	Color(){r=g=b=a=0;};
	Color(int red, int green, int blue, int alpha=255){r=red; g=green; b=blue; a=alpha;};
	inline SDL_Color ToSDLFormat() { SDL_Color color; color.r = (Uint8)r; color.g=(Uint8)g; color.b=(Uint8)b; return color; };
};
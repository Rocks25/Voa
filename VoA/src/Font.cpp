#include "../include/Font.h"
#include "../include/Plane.h"
#include "../include/sdl_invert.h"
#include "../include/TextureManager.h"

Font::Font()
{
}

Font::~Font()
{
}
void Font::Render(char *text,const char *fontpath, Color color, int ptsize)
{
	char buf[255] = {0};
	strcpy(buf,"fonts/");
	strcat(buf,fontpath);
	strcat(buf,".ttf");
	TTF_Font *font = TTF_OpenFont(buf,ptsize);
	if(!font)
	{
		char buf[512];
		sprintf_s(buf,"Error loading font: %s", TTF_GetError());
		MessageBox(NULL,buf,"Font Error!",MB_OK);
	}

	SDL_Surface *initial;
	SDL_Surface *intermediary;
	int w,h;
	GLuint texture;
	
	if(!font)
		return;
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color.ToSDLFormat());
	
	/* Convert the rendered text to a known format */
	w = NextPowerOfTwo(initial->w);
	h = NextPowerOfTwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	/* prepare to render our texture */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	WM->GetRenderer()->GetCurrentShader()->SetUniformValue("tex",0);
	glColor4f(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
	
	/* Draw a quad at location */
	Plane::Render(0,0,(float)w,(float)h);
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
	TTF_CloseFont(font);
}

int Font::NextPowerOfTwo(int x)
{
	double logbase2 = log(x) / log(2);
	return Round(pow(2,ceil(logbase2)));
}

int Font::Round(double x)
{
	return (int)(x + 0.5);
}
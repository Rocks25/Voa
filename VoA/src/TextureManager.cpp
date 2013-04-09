#include "..\include\TextureManager.h"
#include "..\include\window_includes.h"
#include "..\include\sdl_invert.h"



TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
	Cleanup();
}

void TextureManager::Cleanup()
{
	if(_Textures.size()==0)
		return;
	for(unsigned int i=_Textures.size()-1; i>0; i--)
	{
		TextureInfo tex = _Textures[i];
		glDeleteTextures(1,&_Textures[i].pixels);
		//_Textures.pop_back();
	}
}


//bool TextureManager::AddTexture(Texture *tex)
//{
//	for(int i=0;i<_Textures.size();i++)
//	{
//		if(_Textures[i]->GetName() == tex->GetName() || _Textures[i]->GetFilename() == tex->GetFilename())
//			return false;
//	}
//	_Textures.push_back(tex);
//	return true;
//}

bool TextureManager::AddTexture(char *filename,char *name)
{
	if(name==0)
	{
		name=filename;
	}
	if(_Textures.size() > 0)
	{
		for(unsigned int i=0;i<_Textures.size();i++)
		{
			for(unsigned int j=0;j<_Textures[i].names.size();j++)
			{
				if(strcmp(_Textures[i].names[j],name)==0)
					return false;
				else if(strcmp(_Textures[i].filename,filename)==0)
				{
					_Textures[i].names.push_back(name);
					return true;
				}
			}
		}
	}
	TextureInfo tex;
	tex.names.push_back(name);
	tex.filename = filename;


	glGenTextures(1,&tex.pixels);
    SDL_Surface *a = IMG_Load(filename);
	if(!a)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return false;
	}
	SDL_PixelFormat *format = a->format;
	tex.width = a->w;
	tex.height = a->h;
	glBindTexture( GL_TEXTURE_2D, tex.pixels );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );
	SDL_FreeSurface(a);
	_Textures.push_back(tex);
	return true;
}

bool TextureManager::BindTexture(char *name)
{
	for(unsigned int i=0;i<_Textures.size();i++)
	{
		for(unsigned int j=0; j<_Textures[i].names.size(); j++)
		{
			if(strcmp(_Textures[i].names[j],name)==0)
			{
				glBindTexture(GL_TEXTURE_2D, _Textures[i].pixels);
				return true;
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, _Textures[0].pixels);
	return false;
}

TextureInfo TextureManager::GetTextureinfo(char *name)
{
	for(unsigned int i=0;i<_Textures.size();i++)
	{
		for(unsigned int j=0; j<_Textures[i].names.size(); j++)
		{
			if(strcmp(_Textures[i].names[j],name)==0)
			{
				return _Textures[i];
			}
		}
	}
	TextureInfo nullInfo = {0};
	return nullInfo;
}

void TextureManager::ReloadTextures()
{

	std::vector<TextureInfo> tmp = _Textures;
	_Textures.clear();
	for(unsigned int i=0; i<tmp.size();i++)
	{
		glDeleteTextures(1,&tmp[i].pixels);
		for(unsigned int j=0; j<tmp[i].names.size(); j++)
		{
			AddTexture(tmp[i].filename,tmp[i].names[j]);
		}
	}
}
#include "..\include\TextureManager.h"
#include "..\include\window_includes.h"
#include "..\include\Error.h"
#include "..\include\sdl_invert.h"
#include "..\include\WindowManager.h"
#include <string>



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
				{
					char buf[512];
					sprintf(buf,"Duplicate Texture Name '%s'!",name);
					//Error->NewError(buf);
					return false;
				}
				else if(strcmp(_Textures[i].filename,filename)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Filename '%s'!",filename);
					//Error->NewError(buf);
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
		char buf[256]={0};
		sprintf_s(buf,"Error loading image. '%s'!",IMG_GetError());
		//Error->NewError(buf);
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
			char *tmp = _Textures[i].names[j];
			if(strcmp(tmp,name)==0)
			{
				glBindTexture(GL_TEXTURE_2D, _Textures[i].pixels);
				WM->GetRenderer()->GetShaderProgramByName("Main")->SetUniformValue("tex",0);
				WM->GetRenderer()->GetShaderProgramByName("Main")->SetUniformValue("alpha",1);
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
	std::vector<std::vector<char *>> names;
	std::vector<char *> filenames;

	for(unsigned int i=0; i<_Textures.size();i++)
	{
		//Delete all texture data
		glDeleteTextures(1,&_Textures[i].pixels);

		// Grab texture names and filename
		names.push_back(_Textures[i].names);
		filenames.push_back(_Textures[i].filename);
	}
	_Textures.clear(); // delete all textures from array

	for(unsigned int i=0; i<filenames.size();i++)
	{
		for(unsigned int j=0; j<names[i].size(); j++)
		{
			// Add textures back in
			AddTexture(filenames[i],names[i][j]);
		}
	}
}

bool TextureManager::TextureExists(char *name)
{
	for(unsigned int i=0;i<_Textures.size();i++)
	{
		for(unsigned int j=0; j<_Textures[i].names.size(); j++)
		{
			char *tmp = _Textures[i].names[j];
			if(strcmp(tmp,name)==0)
			{
				return true;
			}
		}
	}

	return false;
}
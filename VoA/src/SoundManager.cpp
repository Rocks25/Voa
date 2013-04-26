#include "..\include\SoundManager.h"
#include "..\include\Error.h"


SoundManager::SoundManager(void)
{
}


SoundManager::~SoundManager(void)
{
}

void SoundManager::AddSound(char *filename, char *name)
{
	if(name==0)
	{
		name=filename;
	}
	if(_Sounds.size() > 0)
	{
		for(unsigned int i=0;i<_Sounds.size();i++)
		{
			for(unsigned int j=0;j<_Sounds[i].names.size();j++)
			{
				if(strcmp(_Sounds[i].names[j],name)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Texture Name '%s'!",name);
					//Error->NewError(buf);
					return;
				}
				else if(strcmp(_Sounds[i].filename,filename)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Filename '%s'!",filename);
					//Error->NewError(buf);
					_Sounds[i].names.push_back(name);
					return;
				}
			}
		}
	}
}

void SoundManager::AddMusic(char *filename, char *name)
{
	if(name==0)
	{
		name=filename;
	}
	if(_Music.size() > 0)
	{
		for(unsigned int i=0;i<_Music.size();i++)
		{
			for(unsigned int j=0;j<_Music[i].names.size();j++)
			{
				if(strcmp(_Music[i].names[j],name)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Texture Name '%s'!",name);
					//Error->NewError(buf);
					return;
				}
				else if(strcmp(_Music[i].filename,filename)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Filename '%s'!",filename);
					//Error->NewError(buf);
					_Music[i].names.push_back(name);
					return;
				}
			}
		}
	}
}

void SoundManager::PlaySound(char *name, int fadein)
{
	for(unsigned int i=0;i<_Sounds.size();i++)
	{
		for(unsigned int j=0; j<_Sounds[i].names.size(); j++)
		{
			char *tmp = _Sounds[i].names[j];
			if(strcmp(tmp,name)==0)
			{
				if(_UsedChannels.size() <= 16)
				{
					for(int i=0; i < _UsedChannels.size(); i++)
					{
					}
					//Mix_PlayChannel();
				}
			}
		}
	}
}

void SoundManager::StopSound(char *name, int fadeout)
{
	for(unsigned int i=0;i<_Sounds.size();i++)
	{
		for(unsigned int j=0; j<_Sounds[i].names.size(); j++)
		{
			char *tmp = _Sounds[i].names[j];
			if(strcmp(tmp,name)==0)
			{
				break;
			}
		}
	}
}

void SoundManager::PlayMusic(char *name, int fadein)
{
	for(unsigned int i=0;i<_Music.size();i++)
	{
		for(unsigned int j=0; j<_Music[i].names.size(); j++)
		{
			char *tmp = _Music[i].names[j];
			if(strcmp(tmp,name)==0)
			{
				Mix_FadeInMusic(_Music[i]._Music,-1,fadein);
				return;
			}
		}
	}
}

void SoundManager::StopMusic(int fadeout)
{
	Mix_FadeOutMusic(2000);
}
#include "..\include\SoundManager.h"
#include "..\include\Error.h"


SoundManager::SoundManager(void)
{
	memset(_UsedChannels,0,sizeof(int)*16);
}


SoundManager::~SoundManager(void)
{
}

void SoundManager::AddSound(char *name, char *filename)
{
	if(name==0)
	{
		name=filename;
	}
	if(_Sounds.size() > 0)
	{
		for(unsigned int i=0;i<_Sounds.size();i++)
		{
			for(unsigned int j=0;j<_Sounds[i].Names.size();j++)
			{
				if(strcmp(_Sounds[i].Names[j],name)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Sound Name '%s'!",name);
					Error->NewError(buf);
					return;
				}
				else if(strcmp(_Sounds[i].Filename,filename)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Filename '%s'!",filename);
					Error->NewError(buf);
					_Sounds[i].Names.push_back(name);
					return;
				}
			}
		}
	}
	Sound snd;
	snd.Channel = -1;
	snd.Filename = filename;
	snd.Playing = false;
	snd.Names.push_back(name);
	snd.Chunk = Mix_LoadWAV(filename);
	_Sounds.push_back(snd);
}

void SoundManager::AddMusic(char *name, char *filename)
{
	if(name==0)
	{
		name=filename;
	}
	if(_Music.size() > 0)
	{
		for(unsigned int i=0;i<_Music.size();i++)
		{
			for(unsigned int j=0;j<_Music[i].Names.size();j++)
			{
				if(strcmp(_Music[i].Names[j],name)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Sound Name '%s'!",name);
					Error->NewError(buf);
					return;
				}
				else if(strcmp(_Music[i].Filename,filename)==0)
				{
					char buf[512];
					sprintf(buf,"Duplicate Sound File '%s'!",filename);
					Error->NewError(buf);
					_Music[i].Names.push_back(name);
					return;
				}
			}
		}
	}
	Music mus;
	mus.Filename = filename;
	mus._Music = Mix_LoadMUS(filename);
	mus.Names.push_back(name);
	_Music.push_back(mus);
}

void SoundManager::StartSound(char *name, int fadein, int repeat)
{
	for(unsigned int i=0;i<_Sounds.size();i++)
	{
		for(unsigned int j=0; j<_Sounds[i].Names.size(); j++)
		{
			char *tmp = _Sounds[i].Names[j];
			if(strcmp(tmp,name)==0)
			{
				for(int k=0; k < 16; k++)
				{
					if(_UsedChannels[k]==false)
					{
						Mix_PlayChannel(k,_Sounds[i].Chunk,repeat);
						break;
					}
				}
			}
		}
	}
}

void SoundManager::StopSound(char *name, int fadeout)
{
	for(unsigned int i=0;i<_Sounds.size();i++)
	{
		for(unsigned int j=0; j<_Sounds[i].Names.size(); j++)
		{
			char *tmp = _Sounds[i].Names[j];
			if(strcmp(tmp,name)==0)
			{
				Mix_HaltChannel(_Sounds[i].Channel);
				_Sounds[i].Channel = -1;
				break;
			}
		}
	}
}

void SoundManager::PlayMusic(char *name, int fadein)
{
	for(unsigned int i=0;i<_Music.size();i++)
	{
		for(unsigned int j=0; j<_Music[i].Names.size(); j++)
		{
			char *tmp = _Music[i].Names[j];
			if(strcmp(tmp,name)==0)
			{
				Mix_FadeInMusic(_Music[i]._Music,-1,fadein);
				_CurrentSong = i;
				return;
			}
		}
	}
}

void SoundManager::PlayMusic(int songnum, int fadein)
{
	if(songnum < _Music.size() && songnum >= 0)
	{
		Mix_FadeInMusic(_Music[songnum]._Music,-1,fadein);
		_CurrentSong = songnum;
	}
}

void SoundManager::StopMusic(int fadeout)
{
	Mix_FadeOutMusic(2000);
}

char *SoundManager::GetCurrSongName()
{
	return _Music[_CurrentSong].Names[0];
}

int SoundManager::GetNumSongs()
{
	return _Music.size();
}

int SoundManager::GetNumSounds()
{
	return _Sounds.size();
}

void SoundManager::PlayRandomSong()
{
	int pick = rand()%(GetNumSongs()-1);
	PlayMusic(pick);
}
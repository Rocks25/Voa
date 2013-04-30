#pragma once

#include <vector>
#include <SDL_mixer.h>

class SoundManager
{
private:
	class Sound
	{
	public:
		char *Filename;
		std::vector<char *> Names;
		bool Playing;
		int Channel;
		Mix_Chunk *Chunk;
	};
	class Music
	{
	public:
		char *Filename;
		std::vector<char *> Names;
		Mix_Music *_Music;
	};
	std::vector<Sound> _Sounds;
	std::vector<Music> _Music;
	int _UsedChannels[16];
	int _CurrentSong;
public:
	SoundManager(void);
	~SoundManager(void);
	void AddSound(char *name, char *file);
	void AddMusic(char *name, char *file);
	int GetNumSongs();
	int GetNumSounds();
	char *GetCurrSongName();
	void StartSound(char *name, int fadein=0, int repeat=0);
	void StopSound(char *name, int fadeout=0);
	void PlayMusic(char *name, int fadein=0);
	void PlayMusic(int songnum, int fadein=0);
	void PlayRandomSong();
	void StopMusic(int fadeout=0);
	void ReloadSounds();
};

extern SoundManager *Sounds;
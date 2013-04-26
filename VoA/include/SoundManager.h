#pragma once

#include <vector>
#include <SDL_mixer.h>

class SoundManager
{
private:
	class Sound
	{
	public:
		char *filename;
		std::vector<char *> names;
		Mix_Chunk *Channels[16];
	};
	class Music
	{
	public:
		char *filename;
		std::vector<char *> names;
		Mix_Music *_Music;
	};
	std::vector<Sound> _Sounds;
	std::vector<Music> _Music;
	std::vector<int> _UsedChannels;
public:
	SoundManager(void);
	~SoundManager(void);
	void AddSound(char *file, char *name);
	void AddMusic(char *file, char *name);
	void PlaySound(char *name, int fadein=0);
	void StopSound(char *name, int fadeout=0);
	void PlayMusic(char *name, int fadein=0);
	void StopMusic(int fadeout=0);
	void ReloadSounds();
};


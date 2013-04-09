#pragma once

#include <vector>

typedef struct
{
	char *filename;
	std::vector<char *> names;
	int width;
	int height;
	unsigned int pixels;
} TextureInfo;

class TextureManager
{
private:
	// Private Function Prototypes //
	TextureManager(TextureManager &){};		// Keep Copy constructor private so that it can not be copied
	TextureManager &operator =(int){};

	// Class Variables //
	std::vector<TextureInfo> _Textures;

public:
	TextureManager(void);
	~TextureManager(void);
	void Cleanup();
	//bool AddTexture(Texture *tex);	// Add a texture to the pool
	bool AddTexture(char *filename, char *name=0);
	TextureInfo GetTextureinfo(char *name);
	bool BindTexture(char *name);
	void ReloadTextures();
};

extern TextureManager *TM;
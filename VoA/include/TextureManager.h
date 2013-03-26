#pragma once

#include "vector.h"
#include <vector>

typedef struct
{
	char *filename;
	char *name;
	int width;
	int height;
	unsigned int pixels;
} TextureInfo;

class TextureManager
{
private:
	// Private Function Prototypes //
	TextureManager(void);			// Keep Private so no instantiation is allowed
	~TextureManager(void);			//

	// Class Variables //
	Vector<TextureInfo *> _Textures;
	static TextureManager *_instance;

public:
	static TextureManager *GetSingleton();
	//bool AddTexture(Texture *tex);	// Add a texture to the pool
	bool AddTexture(char *filename, char *name=0);
	TextureInfo *GetTextureinfo(char *name);
	bool BindTexture(char *name);
	void ReloadTextures();
};


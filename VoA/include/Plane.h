#pragma once
#include "../include/Mesh.h"
#include "../include/types.h"

class Plane : public Mesh
{
public:
	Plane(char *name, float width, float height, char *difftex="White", char *alphatex="White");
	virtual ~Plane(void);
	void Render();
	static void Render(float x, float y, float width, float height);
	static void RenderInverted(float x, float y, float width, float height);
	void Init();
	void SetDiffuseTexture(char *name);
	void SetAlphaTexture(char *name);
private:
	//Class Variables
	float _width;
	float _height;
	char _difftex[255], _alphatex[255];

};


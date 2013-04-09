#pragma once
#include "..\include\basicobject.h"
#include "../include/types.h"

class Plane :
	public BasicObject
{
public:
	Plane(float width, float height);
	virtual ~Plane(void);
	void Render();
	static void Render(float x, float y, float width, float height);
	static void RenderInverted(float x, float y, float width, float height);
	void Init();
private:
	//Class Variables
	float _width;
	float _height;

};


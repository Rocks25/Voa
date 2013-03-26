#pragma once

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	void Render();
	void InitShaders();
	void InitFramebuffer();
};


#ifndef SDLWIN_H
#define SDLWIN_H

#include "window_includes.h"
#include "ship.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "Scene.h"
#include "MainMenu.h"

#define TICK_INTERVAL		30

class SDLWin
{
    public:
        SDLWin();
        virtual ~SDLWin();
        int Run();			// Main Loop
        void TimeOut();
    protected:
		// Initialization Routines
		bool Init();
		void InitTextures();
		void InitGeometry();
		void InitKeyActions();
		void ModeDisplay();

		// Per-Frame Routines
        void Loop();		// Main Loop Processing
        void Render();
		void RenderPostProcessing();
        void Events(SDL_Event *event);
		Uint32 TimeLeft();

		// Misc Routines
		void To3D();
		void To2D();
		void TurnLeft();
		void TurnRight();

		// Cleanup
        void Cleanup();

		bool fullscreen;
		bool Perspective;

		int _mouseX, _mouseY;
		float _blurX, _blurY, _blurXrate, _blurYrate;
		bool _blurXinc, _blurYinc;
};

#endif // SDLWIN_H

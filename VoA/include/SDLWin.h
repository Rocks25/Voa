#ifndef SDLWIN_H
#define SDLWIN_H

#include "WindowFunctions.h"
#include "ship.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "Scene.h"
#include "About.h"

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
		void ShaderInit();
		void InitTextures();
		void InitGeometry();
		void InitFramebuffer();
		void InitKeyActions();

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

		About about;
		glm::mat4 perspMat;
		glm::mat4 orthMat;
		glm::mat4 viewMat;
		int _mouseX, _mouseY;
		float _blurX, _blurY, _blurXrate, _blurYrate;
		bool _blurXinc, _blurYinc;
		GLuint fbo, fbo_depth, fbo_texture;
};

#endif // SDLWIN_H

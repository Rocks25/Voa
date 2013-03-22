#ifndef SDLWIN_H
#define SDLWIN_H

#include "myglext.h"
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
        int Run();
        void TimeOut();
    protected:
		bool Init();
        bool SDLInit();
        void glInit();
		void ShaderInit();
        void Loop();
        void Render();
		void RenderPostProcessing();
        void Events(SDL_Event *event);
        void Cleanup();
		Uint32 TimeLeft();
		void To3D();
		void To2D();
		void InitTextures();
		void InitGeometry();
		void InitFramebuffer();
		void InitKeyActions();
		void TurnLeft();
		void TurnRight();

        bool Running;
		bool fullscreen;
		bool Perspective;

		About about;
		Scene scene1;
		GLShaderProgram *spMain;
		GLShaderProgram *spBlurH;
		GLShaderProgram *spBlurV;
		glm::mat4 perspMat;
		glm::mat4 orthMat;
		glm::mat4 viewMat;
		bool keys[323];
		SDL_Surface *window;
		GLuint ptexture[5];
		int _mouseX, _mouseY;
		bool grabmouse;
		float _blurX, _blurY, _blurXrate, _blurYrate;
		bool _blurXinc, _blurYinc;
		GLuint fbo, fbo_depth, fbo_texture;
};

#endif // SDLWIN_H

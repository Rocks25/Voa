#pragma once

#define GL_GLEXT_PROTOTYPES

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image.h>
#elif defined _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Windows.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include <GL/glew.h>
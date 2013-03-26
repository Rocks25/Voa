#ifndef GLSHADER_H
#define GLSHADER_H
#include <windows.h>
#include "../include/WindowFunctions.h"
#include <vector>
#include <string>

class GLShader
{
    public:
		GLShader(int type=GL_VERTEX_SHADER, const char *filename="");
        virtual ~GLShader();
		void Init();
		GLuint GetShaderID();
    protected:
		const char *filename;
		int type;
        GLuint shaderID;
		std::string content;
    private:
};

#endif // GLSHADER_H

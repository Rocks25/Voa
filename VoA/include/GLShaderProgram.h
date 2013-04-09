#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include "GLShaderProgram.h"
#include "GLShader.h"
#include <vector>

class GLShaderProgram
{
    public:
        GLShaderProgram(char *name);
        virtual ~GLShaderProgram();
		void Create();
		GLuint GetProgramID();
		void NewVertexShader(char *filename);
		void NewFragmentShader(char *filename);
		char *GetName();
		void Link();
		void Bind();
		void SetUniformValue(const char *name, int value);
		std::vector<GLShader> GetShaders();
    private:
		char *_Name;
		std::vector<GLShader> _Shaders;
		GLuint programID;
};

#endif // GLSHADERPROGRAM_H

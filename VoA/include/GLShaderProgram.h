#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include "WindowFunctions.h"
#include "GLShader.h"
#include "vector.h"

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
    private:
		char *_Name;
		Vector<GLShader *> _Shaders;
		GLuint programID;
};

#endif // GLSHADERPROGRAM_H

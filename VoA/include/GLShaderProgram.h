#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include "../include/myglext.h"
#include "../include/GLShader.h"
#include <vector>

class GLShaderProgram
{
    public:
        GLShaderProgram();
        virtual ~GLShaderProgram();
		void Create();
		GLuint GetProgramID();
		std::vector<GLShader *> GetShaders();
		void AttachShader(GLShader *shader);
		void Link();
		void Bind();
		void UnBind();
		void SetUniformValue(const char *name, int value);
    protected:
		std::vector<GLShader *> shaders;
		GLuint programID;
};

#endif // GLSHADERPROGRAM_H

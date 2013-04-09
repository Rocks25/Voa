#include "../include/GLShader.h"
#include <string>
#include <direct.h>

GLShader::GLShader(int t, const char *fn)
{
	type = t;
	filename = fn;
}

GLShader::~GLShader()
{
}

void GLShader::Init()
{
	if(type == GL_VERTEX_SHADER || type == GL_GEOMETRY_SHADER || type == GL_TESS_CONTROL_SHADER || type == GL_TESS_EVALUATION_SHADER || type ==  GL_FRAGMENT_SHADER)
	{
		shaderID = glCreateShader(type);
		if(filename!=0)
		{
			FILE *f;
			int n = fopen_s(&f,filename,"r");
			if(n != 0 || f==0)
			{
				char buf[FILENAME_MAX+255]={0};
				char dir[FILENAME_MAX]={0};
				GetCurrentDirectory(FILENAME_MAX,dir);
				sprintf_s(buf,"File '%s' not found! Quitting.Current Location: '%s'.\n",filename,dir);
				MessageBox(NULL,buf,"ERROR!",MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			while(!feof(f))
			{
				char buf[256] = {0};
				int read = fread(buf, 255,1,f);
				content.append(buf);
			}

			fclose(f);

			const char *buf = content.c_str();

			glShaderSource(shaderID,1,&buf,NULL);
			glCompileShader(shaderID);

			GLint result;

			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
			if(result == GL_FALSE) {
				char buf[255];
				sprintf_s(buf, "Unable to compile shader from %s",filename);
				MessageBox(NULL, buf, "Shader Error!",MB_OK);
			}
		}
		else
		{
			MessageBox(NULL,"Filename is empty!", "Shader Error!",MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		MessageBox(NULL, "Shader type unrecognized", "Shader Error!", MB_OK | MB_ICONERROR);
	}
}

GLuint GLShader::GetShaderID()
{
	return shaderID;
}
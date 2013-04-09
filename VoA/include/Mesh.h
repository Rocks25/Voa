#pragma once

#include "WindowManager.h"
#include "GLShaderProgram.h"

class Mesh
{
public:
	Mesh(const char *name);
	virtual ~Mesh(void);
	const char *GetName();
	void SetName(const char *name);
	virtual void Render();
	virtual void Init();
	virtual void InitTextures();
	virtual void InitGeometry();
	virtual void Cleanup();
    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
    glm::vec3 GetRotation();
    void SetPosition(glm::vec3 pos);
    void SetDirection(glm::vec3 dir);
    void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);
	void Translate(glm::vec3 pos);
	void Rotate(glm::vec3 rot);
	void Scale(glm::vec3 scale);

protected:
	const char *_Name;
	GLfloat *_Vertices;
	GLint *_TexCoords;
	GLubyte *_Indeces;
	glm::vec3 _Rotation;
	glm::vec3 _Position;
	glm::vec3 _Scale;
	glm::vec3 _Direction;
};


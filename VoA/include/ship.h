#ifndef SHIP_H
#define SHIP_H

#define GL_GLEXT_PROTOTYPES
#include "types.h"
#include "particlesystem.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "WindowFunctions.h"
#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

class Ship : public Mesh
{
private:
    float size;
    int width;
    int height;
	glm::mat4 modelMat;
	float throttle;
	GLuint _nVBOVertices;
	GLuint _nVBOTexCoords;
	GLuint _nVBOIndeces;
	GLuint _nVBOColors;


public:
	Ship(float size, const char *name);
    ~Ship();
	void Render();
    void Init();
    void SetSize(float size);
    void SetPosition(glm::vec3 pos);
    void SetDirection(glm::vec3 dir);
    void SetRotation(glm::vec3 rot);
	void SetThrottle(float throt);
    float GetSize();
    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
    glm::vec3 GetRotation();
	float GetThrottle();
	glm::vec3 GetBoundingBox();

protected:
    void InitTextures();
	void InitGeometry();
};

#endif // SHIP_H

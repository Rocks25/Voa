#ifndef SHIP_H
#define SHIP_H

#define GL_GLEXT_PROTOTYPES
#include "types.h"
#include "particlesystem.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "window_includes.h"
#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

class Ship : public Mesh
{
private:
    float size;
    int width;
    int height;
	glm::vec3 _Velocity;
	float throttle;
	GLuint _nVBOVertices;
	GLuint _nVBOTexCoords;
	GLuint _nVBOIndeces;
	GLuint _nVBOColors;


public:
	Ship(float size, const char *name);
    ~Ship();
	virtual void Render();
    virtual void Init();
	void SetThrottle(float throt);
    float GetSize();
    void SetSize(float size);
	float GetThrottle();
	glm::vec3 GetBoundingBox();

protected:
    void InitTextures();
	void InitGeometry();
};

#endif // SHIP_H

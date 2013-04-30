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
	glm::vec3 _Velocity;
	glm::vec3 throttle;
	GLuint _nVBOVertices;
	GLuint _nVBOTexCoords;
	GLuint _nVBOIndeces;
	GLuint _nVBOColors;


public:
	Ship(float size, const char *name);
    ~Ship();
	virtual void Render();
    virtual void Init();
	void SetThrottle(glm::vec3 throt);
	virtual float GetCollisionRadius();
    void SetSize(float size);
	void ResetPosition();
	glm::vec3 GetThrottle();
	glm::vec3 GetBoundingBox();
	void Rotate(glm::vec3 pos);

protected:
    void InitTextures();
	void InitGeometry();
};

#endif // SHIP_H

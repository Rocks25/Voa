#ifndef ASTEROID_H
#define ASTEROID_H

#define GL_GLEXT_PROTOTYPES
#include "types.h"
#include "particlesystem.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "window_includes.h"
#include "Mesh.h"
#include "Plane.h"
#include <vector>
#include <glm/glm.hpp>

class Asteroid : public Plane
{
private:
    float size;
    
	char *texture;
	glm::vec3 _Velocity;
	float _delaycollision;
	char _DiffuseTex;
	char _AlphaTex;


public:
	Asteroid(float size, char *name, char *diffusetex, char *alphatex);
    ~Asteroid();
	virtual void Render();
    virtual void Init();
    float GetSize();
	void SetVelocity(glm::vec3 velocity);
	glm::vec3 GetVelocity();
	float GetCollisionRadius();
	float GetCollisionDelay();
	void UpdateCollisionDelay();
	void ResetCollisionDelay();
    void SetSize(float size);
	glm::vec3 GetBoundingBox();
	int width;
    int height;

protected:
    void InitTextures();
	void InitGeometry();
};

#endif // ASTEROID_H

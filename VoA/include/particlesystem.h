#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#define GL_GLEXT_PROTOTYPES
#include "types.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

class ParticleSystem : public Mesh
{
private:
    float   slowdown;
    float   zoom;
	glm::mat4 modelMat;
	glm::mat4 oldModelMat;
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 rotation;
    float   _size;
    float   _strength;
    float   _growthRate;
	GLuint ptexture[2];
    std::vector<particle> newparticles;
	std::vector<particle> particles;

public:
    ParticleSystem(char *name);
    //ParticleSystem(float size, float strength, int numparticles, float growthRate);
	~ParticleSystem();

    void Init(float size, float strength, int numParticles, float growthRate);
	void Render(GLShaderProgram *program, glm::mat4 modelMat=glm::mat4(1.0));
    glm::vec3 GetPositionVec();
    glm::mat4 GetPositionMat();
    glm::vec3 GetDirection();
    glm::vec3 GetRotation();
	float GetStrength();
    void SetPositionVec(glm::vec3 pos);
    void SetPositionMat(glm::mat4 mat);
    void SetDirection(glm::vec3 dir);
    void SetRotation(glm::vec3 rot);
	void SetStrength(float str);
    float GetSize();
    void SetSize(float size);
	void InitTextures();
	void Clear();
};

#endif // PARTICLESYSTEM_H

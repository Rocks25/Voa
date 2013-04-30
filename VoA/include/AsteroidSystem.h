#ifndef ASTEROIDSYSTEM_H
#define ASTEROIDSYSTEM_H

#define GL_GLEXT_PROTOTYPES
#include "types.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include "Asteroid.h"
#include "Entity.h"
#include <vector>
#include <glm/glm.hpp>

class AsteroidSystem : public Entity
{
private:
    int maxAsteroids;
	//std::vector<Asteroid> _Asteroids;

public:
    AsteroidSystem();
    //AsteroidSystem(float size, float strength, int numparticles, float growthRate);
	~AsteroidSystem();

    void Init();
	void Render();
	bool CheckSpawnCollisions(glm::vec3 pos);
	void InitTextures();
	void Clear();
	void Spawn();
	void Update();
	void NewPosition(Asteroid *ast);
	void NewVelocity(Asteroid *ast);
};

//extern AsteroidSystem *AS;

#endif // PARTICLESYSTEM_H

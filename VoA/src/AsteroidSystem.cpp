#include "../include/AsteroidSystem.h"
#include "../include/window_includes.h"
#include "../include/WindowManager.h"
#include "../include/TextureManager.h"
#include "../include/mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../include/GlobalMatrices.h"
#include "../include/Plane.h"
#include "../include/ship.h"
#include "../include/SceneManager.h"


AsteroidSystem::AsteroidSystem() : Entity("Asteroids")
{
	maxAsteroids = 10;
}

AsteroidSystem::~AsteroidSystem()
{
	_Meshes.clear();
}

void AsteroidSystem::Render()
{
	//GLShaderProgram *program = WM->GetRenderer()->GetCurrentShader();
	GLShaderProgram *program = WM->GetRenderer()->GetShaderProgramByName("Main");
	//float newWidth = (width/2.0f)*size;
	//float newHeight = (height/2.0f)*size;

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GMat->UpdateShader();
	glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("Rock1");
	program->SetUniformValue("tex",0);
	glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("Rock1 Alpha");
	program->SetUniformValue("alpha",1);
	GMat->UpdateShader();
	
	for (int loop=0;loop < _Meshes.size();loop++)
	{

		GMat->ModelMatrix()->PushMatrix();
		GMat->ModelMatrix()->LoadIdentity();

		
		Asteroid *ast = (Asteroid *)_Meshes[loop];
		GMat->ModelMatrix()->Translate(ast->GetPosition().x, ast->GetPosition().y, ast->GetPosition().z);
		GMat->ModelMatrix()->Rotate(ast->GetRotation().x,1.0f, 0.0f, 0.0f);
		GMat->ModelMatrix()->Rotate(ast->GetRotation().y,0.0f, 1.0f, 0.0f);
		GMat->ModelMatrix()->Rotate(ast->GetRotation().z,0.0f, 0.0f, 1.0f);

		//GMat->ModelMatrix()->Scale(ast->GetSize(), ast->GetSize(), ast->GetSize());
		
		GMat->UpdateShader();


		//Plane::Render(-ast->width/2.0f,-ast->height/2.0f,(float)ast->width,(float)ast->height);
		//Plane::Render(-((float)256*ast->GetSize())/2.0f,-((float)234*ast->GetSize())/2.0f,(float)256*ast->GetSize(),(float)234*ast->GetSize());
		//Plane::Render(-((float)ast->GetSize())/2.0f,-((float)ast->GetSize())/2.0f,(float)ast->GetSize(),(float)ast->GetSize());

		ast->Render();

		GMat->ModelMatrix()->PopMatrix();
	}

}

void AsteroidSystem::Init()
{
	while(_Meshes.size() <= maxAsteroids)
	{
		char name1[255] = {0};
		char name2[255] = {0};
		sprintf(name1, "Rock%d",rand()%6+1);
		sprintf(name2, "%s Alpha", name1);
		Asteroid *tmp = new Asteroid(64.0f, "Asteroid", name1, name2);
		tmp->SetPosition(glm::vec3((float)(rand() % WM->GetWindowWidth()), (float)(rand() % WM->GetWindowHeight()), 0));
		while(CheckSpawnCollisions(tmp->GetPosition()))
		{
			tmp->SetPosition(glm::vec3((float)(rand() % WM->GetWindowWidth()), (float)(rand() % WM->GetWindowHeight()), 0));
		}
		glm::vec3 tmpVel = glm::vec3((float)(rand() % 200 - 100)/100.0f, (float)(rand() % 200 - 100)/100.0f, 0);
		while (tmpVel.x == 0 || tmpVel.y == 0)
		{
			tmpVel = glm::vec3((float)(rand() % 200 - 100)/100.0f, (float)(rand() % 200 - 100)/100.0f, 0);
		}

		tmp->SetVelocity(glm::vec3(tmpVel));
		_Meshes.push_back(tmp);
	}
}

void AsteroidSystem::InitTextures()
{
	//TM->AddTexture("images/ship.jpg", "Ship");
	//TM->AddTexture("images/ship_alpha.jpg", "Ship_Alpha");
	//TextureInfo info = TM->GetTextureinfo("Ship");
	//TM->AddTexture(_TexFilename);
}

void AsteroidSystem::Clear()
{
	_Meshes.clear();
}

void AsteroidSystem::Spawn()
{
	while(_Meshes.size() <= maxAsteroids)
	{
		char name1[255] = {0};
		char name2[255] = {0};
		sprintf(name1, "Rock%d",rand()%6+1);
		sprintf(name2, "%s Alpha", name1);
		Asteroid *tmp = new Asteroid(64.0f, "Asteroid", name1, name2);
		NewPosition(tmp);
		while(CheckSpawnCollisions(tmp->GetPosition()))
		{
			NewPosition(tmp);
		}

		NewVelocity(tmp);
		_Meshes.push_back(tmp);
	}
}

bool AsteroidSystem::CheckSpawnCollisions(glm::vec3 pos)
{
	if(_Meshes.size()<=0)
		return false;
	for(int i=0; i<_Meshes.size()-1; i++)
	{
		Asteroid *m1 = (Asteroid *)_Meshes[i];
		if(m1->GetCollisionDelay()>0)
		{
			m1->UpdateCollisionDelay();
			continue;
		}
		glm::vec3 m1pos = m1->GetPosition();
		float m1size = m1->GetCollisionRadius();
			
		float distancesq = sqrt(pow(pos.x-m1pos.x,2) + pow(pos.y-m1pos.y,2));
		float sizesq = 3*m1size;

		if(distancesq < sizesq)
		{
			return true;
		}
		

		/*Ship *ship = (Ship *)SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("Player Ship");
		glm::vec3 spos = ship->GetPosition();

		distancesq = sqrt(pow(pos.x-spos.x,2) + pow(pos.y-spos.y,2));
		sizesq = m1size+50.0f;

		if(distancesq < sizesq)
		{
			return true;
		}*/

	}
	return false;
}

void AsteroidSystem::Update()
{
	for (int loop=0; loop < _Meshes.size(); loop++)
	{
		Asteroid *ast = (Asteroid *)_Meshes[loop];
		if(ast->GetPosition().x > WM->GetWindowWidth() || ast->GetPosition().x < 0)
		{
			_Meshes.erase(_Meshes.begin()+loop);
		} else if(ast->GetPosition().y > WM->GetWindowHeight() || ast->GetPosition().y < 0)
		{
			_Meshes.erase(_Meshes.begin()+loop);
		}
		else
		{
			ast->SetPosition(ast->GetPosition() += ast->GetVelocity());
		}
	}
	//_Meshes.shrink_to_fit();
}

void AsteroidSystem::NewPosition(Asteroid *ast)
{
	int axis = rand()%1;
	glm::vec3 tmppos = ast->GetPosition();
	if(axis == 0)
	{
		if((rand()%2)==0)
			tmppos.x=0;
		else
			tmppos.x=WM->GetWindowWidth();
		tmppos.y = rand()%WM->GetWindowHeight();
	}
	else
	{
		if((rand()%2)==0)
			tmppos.y=0;
		else
			tmppos.y=WM->GetWindowHeight();
		tmppos.x = rand()%WM->GetWindowWidth();
	}
	ast->SetPosition(tmppos);
}

void AsteroidSystem::NewVelocity(Asteroid *ast)
{
	glm::vec3 pos = ast->GetPosition();
	glm::vec3 tmpVel;
	if(pos.x == 0)
	{
		tmpVel = glm::vec3((float)(rand() % 99 + 1)/100.0f, (float)(rand() % 200 - 100)/100.0f, 0);
	}
	else if(pos.x == WM->GetWindowWidth())
	{
		tmpVel = glm::vec3((float)(rand() % 99 - 100)/100.0f, (float)(rand() % 200 - 100)/100.0f, 0);
	}

	if(pos.y == 0)
	{
		tmpVel = glm::vec3((float)(rand() % 200 - 100)/100.0f, (float)(rand() % 99 - 100)/100.0f, 0);
	}
	else if(pos.y == WM->GetWindowHeight())
	{
		tmpVel = glm::vec3((float)(rand() % 200 - 100)/100.0f, (float)(rand() % 99 + 1)/100.0f, 0);
	}
	//glm::vec3 tmpVel = glm::vec3((float)(rand() % 200 - 100)/100.0f, (float)(rand() % 200 - 100)/100.0f, 0);
	ast->SetVelocity(tmpVel);
}
#include "../include/ship.h"
#include "../include/GLShader.h"
#include "../include/TextureManager.h"
#include "../include/WindowManager.h"
#include "../include/GlobalMatrices.h"
#include "../include/Plane.h"
#include "../include/Error.h"
#include "../include/SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Ship::Ship(float size, const char *name) : Mesh(name)
{
    this->size = size;
}
Ship::~Ship()
{
	/*pEngineL.clear();
	pEngineR.clear();*/
}

void Ship::Init()
{
    InitTextures();
	InitGeometry();
}

void Ship::Render()
{
	GLShaderProgram *program = WM->GetRenderer()->GetShaderProgramByName("Main");

	/*pEngineL.SetSize(throttle);
	pEngineR.SetSize(throttle);
	pEngineL.SetStrength(throttle*(-35));
	pEngineR.SetStrength(throttle*(-35));
	
	pEngineL.SetPositionMat(glm::translate(modelMat,glm::vec3(-newWidth/2.0f,-0.1,0.0)));
	pEngineR.SetPositionMat(glm::translate(modelMat,glm::vec3(newWidth/2.0f,-0.1,0.0)));
	pEngineL.Render(program, modelMat);
	pEngineR.Render(program, modelMat);*/

	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*GLfloat vertices[] = {
		+newWidth,+newHeight,0.0f,
		-newWidth,+newHeight,0.0f,
		+newWidth,-newHeight,0.0f,

		+newWidth,-newHeight,0.0f,
		-newWidth,+newHeight,0.0f,
		-newWidth,-newHeight,0.0f};*/
	//GMat->ModelMatrix()->PushMatrix();

    glActiveTexture(GL_TEXTURE0);
	TM->BindTexture("Ship");
	GMat->UpdateShader();

    glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("Ship_Alpha");
	GMat->UpdateShader();

	Plane::Render(-158/2.0f,-110/2.0f,(float)158,(float)110);

	//GMat->ModelMatrix()->PopMatrix();
}

void Ship::InitTextures()
{
	TM->AddTexture("images/Ship/ship.jpg", "Ship");
	TM->AddTexture("images/Ship/ship_alpha.jpg", "Ship_Alpha");
}

float Ship::GetSize()
{
    return size;
}

void Ship::SetSize(float size)
{
    float oldsize_s=this->size;
    //float oldsize_p=pEngineL.GetSize();
    float newsize_s = oldsize_s - (oldsize_s-size);
    //float newsize_p = oldsize_p + (oldsize_s-size)*1/(1/oldsize_s-1/oldsize_p);

    this->size=newsize_s;
    //pEngineL.SetSize(newsize_p);
    //pEngineR.SetSize(newsize_p);
}

void Ship::SetThrottle(glm::vec3 throt)
{
	throttle=throt;
}

glm::vec3 Ship::GetThrottle()
{
	return throttle;
}

void Ship::InitGeometry()
{
	SetPosition(glm::vec3(WM->GetWindowWidth()/2.0f,WM->GetWindowHeight()/2.0f,0));
	_Rotation = glm::vec3(0,0,90);
}

glm::vec3 Ship::GetBoundingBox()
{
	return glm::vec3(64,64,0);
}

void Ship::Rotate(glm::vec3 rot)
{
	_Rotation += rot;
	Mesh *dummy = SM->GetSceneByName("Main")->GetEntityByName("Player")->GetMeshByName("ShipDummy");
	glm::mat4 tmpmat = glm::translate(dummy->GetPosition());
	tmpmat = glm::rotate(tmpmat,_Rotation.x,glm::vec3(1,0,0));
	tmpmat = glm::rotate(tmpmat,_Rotation.y,glm::vec3(0,1,0));
	tmpmat = glm::rotate(tmpmat,_Rotation.z,glm::vec3(0,0,1));
	glm::vec3 tmpdir = glm::vec3(tmpmat*glm::vec4(1,1,1,0));
	_Direction = tmpdir/sqrt((tmpdir.x*tmpdir.x)+(tmpdir.y*tmpdir.y)+(tmpdir.z*tmpdir.z));
	dummy->SetPosition(glm::vec3(tmpmat*glm::vec4(1,1,1,1)));
}
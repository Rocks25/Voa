#include "../include/Asteroid.h"
#include "../include/GLShader.h"
#include "../include/TextureManager.h"
#include "../include/WindowManager.h"
#include "../include/GlobalMatrices.h"
#include "../include/Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Asteroid::Asteroid(float size, char *name, char *diffusetex, char *alphatex) : Plane(name)
{
    this->size = size;
	strncpy(_difftex, diffusetex, 254);
	strncpy(_alphatex, alphatex, 254);
}
Asteroid::~Asteroid()
{
	/*pEngineL.clear();
	pEngineR.clear();*/
}

void Asteroid::Init()
{
    InitTextures();
	InitGeometry();
}

float Asteroid::GetCollisionRadius()
{
	return size/2.0f;
}

float Asteroid::GetCollisionDelay()
{
	return _delaycollision;
}

void Asteroid::ResetCollisionDelay()
{
	_delaycollision = 60.0f/sqrt(GetVelocity().x*GetVelocity().x+GetVelocity().y*GetVelocity().y);
	//_delaycollision = 600.0f;
}


void Asteroid::UpdateCollisionDelay()
{
	if(_delaycollision > 0)
	{
		_delaycollision--;
		if(_delaycollision < 0)
			_delaycollision = 0;
	}
}

void Asteroid::Render()
{
	GLShaderProgram *program = WM->GetRenderer()->GetCurrentShader();
    float newWidth = (width/2.0f)*size;
    float newHeight = (height/2.0f)*size;

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
	GMat->ModelMatrix()->PushMatrix();

	GMat->ModelMatrix()->Translate(_Position.x, _Position.y, _Position.z);
	GMat->ModelMatrix()->Rotate(_Rotation.x,1.0f, 0.0f, 0.0f);
	GMat->ModelMatrix()->Rotate(_Rotation.y,0.0f, 1.0f, 0.0f);
	GMat->ModelMatrix()->Rotate(_Rotation.z,0.0f, 0.0f, 1.0f);

	GMat->ModelMatrix()->Scale(_Scale);

	GMat->UpdateShader();
    glActiveTexture(GL_TEXTURE0);
	TM->BindTexture(_difftex);

    glActiveTexture(GL_TEXTURE1);
	TM->BindTexture(_alphatex);

	GMat->UpdateShader();

	Plane::Render(-size/2.0f,-size/2.0f,(float)size,(float)size);

	GMat->ModelMatrix()->PopMatrix();
	GMat->UpdateShader();
}

void Asteroid::InitTextures()
{
	
}

float Asteroid::GetSize()
{
    return size;
}

void Asteroid::SetSize(float size)
{
    float oldsize_s=this->size;
    //float oldsize_p=pEngineL.GetSize();
    float newsize_s = oldsize_s - (oldsize_s-size);
    //float newsize_p = oldsize_p + (oldsize_s-size)*1/(1/oldsize_s-1/oldsize_p);

    this->size=newsize_s;
    /*pEngineL.SetSize(newsize_p);
    pEngineR.SetSize(newsize_p);*/
}

void Asteroid::InitGeometry()
{
	
    //float newWidth = (float)width/height*size;
    //float newHeight = (float)height/width*size;
	float newWidth = size, newHeight = size;

	GLfloat vertices[] = {
		+newWidth,+newHeight,0.0f,
		-newWidth,+newHeight,0.0f,
		+newWidth,-newHeight,0.0f,
		-newWidth,-newHeight,0.0f};

	GLubyte indeces[] = {
		0,1,2,
		2,1,3};

	GLfloat texCoords[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f};

	GLfloat colors[] = {
		0.6f,0.3f,0.0f,1.0f,
		0.7f,0.2f,0.0f,1.0f,
		0.0f,0.4f,0.2f,1.0f,
		1.0f,0.0f,0.8f,1.0f};


    // Load The Data
    glBufferData( GL_ARRAY_BUFFER, 4*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
 
    // Generate And Bind The Texture Coordinate Buffer

    // Load The Data
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, 2*4*sizeof(GLfloat), texCoords, GL_STATIC_DRAW_ARB );

    // Generate And Bind The Index Buffer
          // Get A Valid Name
    // Load The Data
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indeces, GL_STATIC_DRAW );
 
	// Generate And Bind The Vertex Color Buffer

    // Load The Data
	glBufferData( GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), colors, GL_STATIC_DRAW );

	//SetPosition(glm::vec3(0,0,0));
	SetPosition(glm::vec3(WM->GetWindowWidth()/2.0f-(width*_Scale.x)/2.0f,WM->GetWindowHeight()/2.0f-(height*_Scale.y)/2.0f,0));
	SetRotation(glm::vec3(0,0,0));
	SetScale(glm::vec3(.25,.25,.25));
}

glm::vec3 Asteroid::GetBoundingBox()
{
	return glm::vec3(width,height,0);
}

void Asteroid::SetVelocity(glm::vec3 velocity)
{
	_Velocity = velocity;
}

glm::vec3 Asteroid::GetVelocity()
{
	return _Velocity;
}
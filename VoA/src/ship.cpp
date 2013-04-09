#include "../include/ship.h"
#include "../include/GLShader.h"
#include "../include/TextureManager.h"
#include "../include/WindowManager.h"
#include "../include/GlobalMatrices.h"
#include "../include/Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Ship::Ship(float size, const char *name) : Mesh(name)
{
    this->size = size;
	throttle=0.0f;
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
	TM->BindTexture("Ship");
    program->SetUniformValue("tex",0);

    glActiveTexture(GL_TEXTURE1);
	TM->BindTexture("Ship_Alpha");
    program->SetUniformValue("alpha",1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_INDEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER,_nVBOVertices);
	glVertexPointer(3,GL_FLOAT,0,NULL);
	glBindBuffer(GL_ARRAY_BUFFER, _nVBOTexCoords);
	glTexCoordPointer(2,GL_FLOAT, 0, (char *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, _nVBOColors);
	glColorPointer(4,GL_FLOAT, 0, NULL);
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _nVBOIndeces);
	glIndexPointer(GL_UNSIGNED_BYTE,0,NULL);*/

	GMat->UpdateShader();
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	//glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	Plane::Render(-width/2.0f,-height/2.0f,(float)width,(float)height);

	GMat->ModelMatrix()->PopMatrix();
}

void Ship::InitTextures()
{
	TM->AddTexture("images/ship.jpg", "Ship");
	TM->AddTexture("images/ship_alpha.jpg", "Ship_Alpha");
	TextureInfo info = TM->GetTextureinfo("Ship");
	width = info.width;
	height = info.height;
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
    /*pEngineL.SetSize(newsize_p);
    pEngineR.SetSize(newsize_p);*/
}

void Ship::SetThrottle(float throt)
{
	throttle=throt;
}

float Ship::GetThrottle()
{
	return throttle;
}

void Ship::InitGeometry()
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

	glGenBuffers( 1, &_nVBOVertices );                  // Get A Valid Name
    glBindBuffer( GL_ARRAY_BUFFER, _nVBOVertices );         // Bind The Buffer
    // Load The Data
    glBufferData( GL_ARRAY_BUFFER, 4*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
 
    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB( 1, &_nVBOTexCoords );                 // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, _nVBOTexCoords );        // Bind The Buffer
    // Load The Data
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, 2*4*sizeof(GLfloat), texCoords, GL_STATIC_DRAW_ARB );

    // Generate And Bind The Index Buffer
    glGenBuffers( 1, &_nVBOIndeces );                 // Get A Valid Name
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _nVBOIndeces );        // Bind The Buffer
    // Load The Data
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indeces, GL_STATIC_DRAW );
 
	// Generate And Bind The Vertex Color Buffer
    glGenBuffers( 1, &_nVBOColors );                 // Get A Valid Name
	glBindBuffer( GL_ARRAY_BUFFER, _nVBOColors );        // Bind The Buffer
    // Load The Data
	glBufferData( GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat), colors, GL_STATIC_DRAW );

	//SetPosition(glm::vec3(0,0,0));
	SetPosition(glm::vec3(WM->GetWindowWidth()/2.0f-(width*_Scale.x)/2.0f,WM->GetWindowHeight()/2.0f-(height*_Scale.y)/2.0f,0));
	SetRotation(glm::vec3(0,0,0));
	SetScale(glm::vec3(.25,.25,.25));
}

glm::vec3 Ship::GetBoundingBox()
{
	return glm::vec3(width,height,0);
}
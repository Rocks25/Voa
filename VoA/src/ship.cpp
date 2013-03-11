#include "../include/ship.h"
#include "../include/GLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Ship::Ship(float size, const char *name) : Mesh(name), modelMat(1.0f)
{
    this->size = size;
	throttle=0.0f;
}
Ship::~Ship()
{
    glDeleteTextures(2,texture);/*
	pEngineL.Clear();
	pEngineR.Clear();*/
}

void Ship::Init()
{
    InitTextures();
	InitGeometry();
}

void Ship::Render(GLShaderProgram *program)
{
	program->Bind();
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

	int modelMatrixLoc = glGetUniformLocation(program->GetProgramID(),"modelMat");
	/*GLfloat vertices[] = {
		+newWidth,+newHeight,0.0f,
		-newWidth,+newHeight,0.0f,
		+newWidth,-newHeight,0.0f,

		+newWidth,-newHeight,0.0f,
		-newWidth,+newHeight,0.0f,
		-newWidth,-newHeight,0.0f};*/
	glUniformMatrix4fv(modelMatrixLoc,1,GL_FALSE,&modelMat[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    program->SetUniformValue("tex",0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
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

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	//glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	/*
	int error = glGetError();

	if(error != GL_NO_ERROR)
	{
		if(error == GL_INVALID_VALUE)
			MessageBox(NULL,"Invalid Value","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_INVALID_ENUM)
			MessageBox(NULL,"Invalid Enumerator","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_INVALID_OPERATION)
			MessageBox(NULL,"Invalid Operation","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
			MessageBox(NULL,"Invalid Framebuffer Operation","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_OUT_OF_MEMORY)
			MessageBox(NULL,"Out Of Memory","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_STACK_UNDERFLOW)
			MessageBox(NULL,"Stack Underflow","Error drawing elements",MB_OK|MB_ICONERROR);
		else if(error == GL_STACK_OVERFLOW)
			MessageBox(NULL,"Stack Overflow","Error drawing elements",MB_OK|MB_ICONERROR);
		else
		{
			char result[512] = {0};
			sprintf(result,"Unknown OpenGL Error '%d' Occured!",(int)glGetError());
			MessageBox(NULL,result,"OpenGL Error!",MB_OK|MB_ICONERROR);
		}
		
	}*/
}

void Ship::InitTextures()
{
	glGenTextures(2,texture);

	//Base Image for Ship
    SDL_Surface *a = IMG_Load("images/ship.jpg");
	SDL_PixelFormat *format = a->format;
	if(a==0)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image. %s!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[0] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGBA, GL_UNSIGNED_BYTE, a->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, a->w, a->h, GL_RGB, GL_UNSIGNED_BYTE, a->pixels );

	//Alpha image for Ship
    SDL_Surface *b = IMG_Load("images/ship_alpha.jpg");
	format = b->format;
	if(b==0)
	{
		char buf[255]={0};
		sprintf_s(buf,"Error loading image '%s'!",IMG_GetError());
		MessageBox(NULL,buf,"Init Error",MB_OK|MB_ICONERROR);
		return;
	}
    glBindTexture( GL_TEXTURE_2D, texture[1] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	if(format->Amask)
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGBA, GL_UNSIGNED_BYTE, b->pixels );
	else
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, b->w, b->h, GL_RGB, GL_UNSIGNED_BYTE, b->pixels );

	width=(float)a->w/sqrt((float)a->w*a->w+(float)a->h*a->h);
	height=(float)a->h/sqrt((float)a->w*a->w+(float)a->h*a->h);
	SDL_FreeSurface(b);
	SDL_FreeSurface(a);
}

float Ship::GetSize()
{
    return size;
}

glm::vec3 Ship::GetDirection()
{
	glm::vec4 dir = -modelMat*glm::vec4(1.0f,1.0f,1.0f,0.0f);
    return glm::vec3(dir.x,dir.y,dir.z);
}

glm::vec3 Ship::GetPosition()
{
	glm::vec4 pos = modelMat*glm::vec4(1.0f);
    return glm::vec3(pos.x,pos.y,pos.z);
}

glm::vec3 Ship::GetRotation()
{
	float heading = atan(modelMat[0][1]/modelMat[0][0]);
	float bank = atan(modelMat[1][2]/modelMat[2][2]);
	float attitude = asin(-modelMat[0][2]);
	return glm::vec3(attitude,bank,heading);
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

void Ship::SetDirection(glm::vec3 dir)
{
    //direction=dir;
}

void Ship::SetPosition(glm::vec3 pos)
{
	modelMat = glm::translate(modelMat, pos);
}

void Ship::SetRotation(glm::vec3 rot)
{
	glm::mat4 tmpPos;
	tmpPos = glm::rotate(rot.x,glm::vec3(1,0,0));
	tmpPos = glm::rotate(rot.y,glm::vec3(0,1,0));
	tmpPos = glm::rotate(rot.z,glm::vec3(0,0,1));
	modelMat *= tmpPos;
	/*pEngineL.SetPositionMat(tmpPos*pEngineL.GetPositionMat());
	pEngineR.SetPositionMat(tmpPos*pEngineR.GetPositionMat());*/
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
	
    float newWidth = width*size;
    float newHeight = height*size;

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
}

glm::vec3 Ship::GetBoundingBox()
{
	return glm::vec3(width,height,0);
}
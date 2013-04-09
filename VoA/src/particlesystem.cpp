#include "../include/particlesystem.h"
#include "../include/window_includes.h"
#include "../include/WindowManager.h"
#include "../include/TextureManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

static GLfloat colors[12][3]=               // Rainbow Of Colors
{
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};


ParticleSystem::ParticleSystem(char *name) : Mesh(name)
{
    //Init(.15f,30.0f,400,1.125f);
}

ParticleSystem::~ParticleSystem()
{
	glDeleteTextures(1,_Texture);
}

void ParticleSystem::Render()
{
	GLShaderProgram *program = WM->GetRenderer()->GetCurrentShader();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	program->Bind();
    
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _Texture[0]);
    program->SetUniformValue("tex",0);
	
	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _Texture[1]);
	program->SetUniformValue("alpha",1);

	int modelMatLoc = glGetUniformLocation(program->GetProgramID(), "modelMat");
	//glUniformMatrix4fv(modelMatLoc,1,GL_FALSE,&_oldModelMat[0][0]);
	glm::mat4 tmpMat = glm::rotate(_Rotation.x, glm::vec3(1,0,0));
	tmpMat = glm::rotate(_Rotation.y, glm::vec3(0,1,0));
	tmpMat = glm::rotate(_Rotation.z, glm::vec3(0,0,1));
	float heading = atan(tmpMat[0][1]/tmpMat[0][0]);
	float bank = atan(tmpMat[1][2]/tmpMat[2][2]);
	float attitude = asin(-tmpMat[0][2]);
	glm::vec3 tmpRot = glm::vec3(attitude,bank,heading);

    for (unsigned int loop=0;loop<_Particles.size();loop++)                   // Loop Through All The Particles
    {
        int col=0;
        if (_Particles[loop].active)                  // If The Particle Is Active
        {
            glLoadIdentity();
            float x=_Particles[loop].pos.x;               // Grab Our Particle X Position
            float y=_Particles[loop].pos.y;               // Grab Our Particle Y Position
            float z=_Particles[loop].pos.z;              // Particle Z Pos + Zoom
            float size=_Particles[loop].size;

            // Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
            //glColor4f(_Particles[loop].color.r,_Particles[loop].color.g,_Particles[loop].color.b,_Particles[loop].life);
            glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

            glTexCoord2d(1,1); glVertex3f(x+size,y+size,z); // Top Right
            glTexCoord2d(0,1); glVertex3f(x-size,y+size,z); // Top Left
            glTexCoord2d(1,0); glVertex3f(x+size,y-size,z); // Bottom Right
            glTexCoord2d(0,0); glVertex3f(x-size,y-size,z); // Bottom Left

            glEnd();                        // Done Building Triangle Strip
            _Particles[loop].pos.x+=(float)_Particles[loop].dir.x/(_Slowdown*1000.0f);     // Move On The X Axis By X Speed
            _Particles[loop].pos.y+=(float)_Particles[loop].dir.y/(_Slowdown*1000.0f);     // Move On The Y Axis By Y Speed
            _Particles[loop].pos.z+=(float)_Particles[loop].dir.z/(_Slowdown*1000.0f);     // Move On The Z Axis By Z Speed
            _Particles[loop].dir.x+=_Particles[loop].grav.x;                              // Take Pull On X Axis Into Account
            _Particles[loop].dir.y+=_Particles[loop].grav.y;                              // Take Pull On Y Axis Into Account
            _Particles[loop].dir.z+=_Particles[loop].grav.z;                              // Take Pull On Z Axis Into Account

            _Particles[loop].size*=(_Particles[loop].life);

            _Particles[loop].life-=_Particles[loop].fade;       // Reduce Particles Life By 'Fade'


            if (_Particles[loop].life<=0.0f)                    // If Particle Is Burned Out
            {
				glm::vec4 pos = tmpMat * glm::vec4(0,0,0,1);
				srand(SDL_GetTicks());
                _Particles[loop].life=1.0f;               // Give It New Life
                _Particles[loop].fade=float(rand()%100)/1000.0f+0.003f;   // Random Fade Value

                //_Particles[loop].pos.x=pos.x * ((rand()%100)/400.0f)-0.125f;               // Center On X Axis
				_Particles[loop].pos.x=pos.x;
                _Particles[loop].pos.y=pos.y;               // Center On Y Axis
                _Particles[loop].pos.z=pos.z;               // Center On Z Axis

				glm::vec4 rot = glm::rotate(45.0f, glm::vec3(0,0,1))*
					glm::rotate(tmpRot.x,glm::vec3(1,0,0))*
					glm::rotate(tmpRot.y, glm::vec3(0,1,0))*
					glm::rotate(tmpRot.z, glm::vec3(0,0,1))*
					//glm::translate(glm::vec3(pos.x,pos.y,pos.z))*
					glm::vec4(1.0);

				rot = tmpMat*rot;

                //_Particles[loop].dir.x=cos(pos.x/pos.y)+((rand()%1000)/100.0f-5.0f)*rot.x;  // X Axis Speed And Direction
				_Particles[loop].dir.x=rot.x*_strength+float((rand()%10));
                //_Particles[loop].dir.y=sin(pos.x/pos.y)+((rand()%100)/60.0f)*rot.y*_strength;  // Y Axis Speed And Direction
				_Particles[loop].dir.y=rot.y*_strength+float((rand()%10));
                _Particles[loop].dir.z=0.0f;     // Z Axis Speed And Direction

                _Particles[loop].color.r=colors[col][0];            // Select Red From Color Table
                _Particles[loop].color.g=colors[col][1];            // Select Green From Color Table
                _Particles[loop].color.b=colors[col][2];            // Select Blue From Color Table

                _Particles[loop].size=_size*((rand()%100)/200.0f+0.5f);

				_NewParticles.push_back(_Particles[loop]);
				_Particles.erase(_Particles.begin()+loop);
            }
        }
    }


	for(unsigned int loop=0;loop<_NewParticles.size();loop++)
	{
		if(_NewParticles[loop].active)
		{
			float x=_Particles[loop].pos.x;               // Grab Our Particle X Position
			float y=_Particles[loop].pos.y;               // Grab Our Particle Y Position
			float z=_Particles[loop].pos.z;              // Particle Z Pos + Zoom
			float size=_Particles[loop].size;

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			//glColor4f(_Particles[loop].color.r,_Particles[loop].color.g,_Particles[loop].color.b,_Particles[loop].life);
			glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

			glTexCoord2d(1,1); glVertex3f(x+size,y+size,z); // Top Right
			glTexCoord2d(0,1); glVertex3f(x-size,y+size,z); // Top Left
			glTexCoord2d(1,0); glVertex3f(x+size,y-size,z); // Bottom Right
			glTexCoord2d(0,0); glVertex3f(x-size,y-size,z); // Bottom Left

			glEnd();                        // Done Building Triangle Strip

			_NewParticles[loop].pos.x+=(float)_Particles[loop].dir.x/(_Slowdown*1000.0f);     // Move On The X Axis By X Speed
			_NewParticles[loop].pos.y+=(float)_Particles[loop].dir.y/(_Slowdown*1000.0f);     // Move On The Y Axis By Y Speed
			_NewParticles[loop].pos.z+=(float)_Particles[loop].dir.z/(_Slowdown*1000.0f);     // Move On The Z Axis By Z Speed

			_NewParticles[loop].size*=(_Particles[loop].life);

			_NewParticles[loop].life-=_Particles[loop].fade;       // Reduce Particles Life By 'Fade'
		}

		_Particles.push_back(_NewParticles[loop]);
		_NewParticles.erase(_NewParticles.begin()+loop);
	}
	/*
	glUniformMatrix4fv(modelMatLoc,1,GL_FALSE,&modelMat[0][0]);
	float size = _size/2.0;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
	glColor4f(0.0f,0.0f,0.0f,0.5f);

	glBegin(GL_TRIANGLE_STRIP);             // Build Quad From A Triangle Strip

    glTexCoord2d(1,1); glVertex3f(+size,+size,0.0f); // Top Right
    glTexCoord2d(0,1); glVertex3f(-size,+size,0.0f); // Top Left
    glTexCoord2d(1,0); glVertex3f(+size,-size,0.0f); // Bottom Right
    glTexCoord2d(0,0); glVertex3f(-size,-size,0.0f); // Bottom Left

    glEnd(); 
	*/
	glActiveTexture(GL_TEXTURE0);
}

void ParticleSystem::Init()
{
}

void ParticleSystem::Setup(float size, float strength, int numParticles, float growthRate)
{
    _Position = glm::vec3(0,0,0);
    _Slowdown=0.50f;
    _Zoom=-40.0f;
    _size=size;
    _strength=strength;
    _growthRate=growthRate;
    for (int loop=0;loop<numParticles;loop++)           // Initialize All The particles
    {
        particle p;
        p.active=true;									// Make All The Particles Active
        p.life=1.0f;									// Give All The Particles Full Life
        p.fade=float(rand()%100)/1000.0f+0.003f;		// Random Fade Speed
        p.size=.1f;									// Set the size
        p.color.r=colors[loop*(12/numParticles)][0];	// Select Red Rainbow Color
        p.color.g=colors[loop*(12/numParticles)][1];	// Select Red Rainbow Color
        p.color.b=colors[loop*(12/numParticles)][2];	// Select Red Rainbow Color
        p.pos.x=0;									// Set the horizontal position to the horizontal position of the emitter
        p.pos.y=0;									// Set the vertical position to the vertical position of the emitter
        p.pos.z=0;									// Set the Z position to the Z position of the emitter
        //p.dir.x=float((rand()%50)-26.0f)*10.0f;			// Random Speed On X Axis
        //p.dir.y=float((rand()%50)-25.0f)*10.0f;			// Random Speed On Y Axis
        //p.dir.z=float((rand()%50)-25.0f)*10.0f;			// Random Speed On Z Axis
		p.dir.x=0;			// Random Speed On X Axis
        p.dir.y=float(-rand()%50)*10.0f;			// Random Speed On Y Axis
        p.dir.z=0;			// Random Speed On Z Axis
        p.grav.x=0.0f;									// Set Horizontal Pull To Zero
        p.grav.y=0.0f;									// Set Vertical Pull to Zero
        p.grav.z=0.0f;									// Set Pull On Z Axis To Zero
        _Particles.push_back(p);							// Add particle to the system
    }
}

glm::vec3 ParticleSystem::GetPositionVec()
{
    return _Position;
}

glm::mat4 ParticleSystem::GetPositionMat()
{
	//return *modelMat;
	return glm::mat4(0);
}

glm::vec3 ParticleSystem::GetDirection()
{
    return glm::vec3();
}

glm::vec3 ParticleSystem::GetRotation()
{
    return _Rotation;
}

void ParticleSystem::SetRotation(glm::vec3 x)
{
    _Rotation=x;
}

float ParticleSystem::GetSize()
{
    return _size;
}

void ParticleSystem::SetSize(float size)
{
    _size=size;
}

void ParticleSystem::SetPositionVec(glm::vec3 position)
{
    //_Position+=position;
}

void ParticleSystem::SetPositionMat(glm::mat4 mat)
{
    //modelMat = &mat;
}

void ParticleSystem::SetDirection(glm::vec3 dir)
{
    _Direction = dir;
}

void ParticleSystem::InitTextures()
{
	//TM->AddTexture(_TexFilename);
}

float ParticleSystem::GetStrength()
{
	return _strength;
}

void ParticleSystem::SetStrength(float str)
{
	_strength=str;
}

void ParticleSystem::Clear()
{
	_Particles.clear();
}
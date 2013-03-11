#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>

class Color3F
{
public:
    float r;
    float g;
    float b;
    Color3F(){r=g=b=0.0;};
    Color3F(float r,float g,float b){this->r=r;this->g=g;this->b=b;};
};

typedef struct
{
    bool    active;                 // Active (Yes/No)
    float   life;                   // Particle Life
    float   fade;                   // Fade Speed
    float   size;                   // Particle Size
    Color3F color;              // Color
    glm::vec3 pos;             // Position
    glm::vec3 dir;             // Direction and Speed
    glm::vec3 grav;            // Gravity
}particle;


#endif // TYPES_H

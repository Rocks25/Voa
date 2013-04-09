#pragma once

#include "Matrix.h"
#include "Camera.h"

class GlobalMatrices
{
private:
	Matrix *_ModelMat;
	Matrix *_ViewMat;
	Matrix *_ProjMat;
public:
	GlobalMatrices(void);
	~GlobalMatrices(void);
	Matrix *ModelMatrix();
	Matrix *ViewMatrix();
	Matrix *ProjectionMatrix();
	void UpdateShader();
};

extern GlobalMatrices *GMat;
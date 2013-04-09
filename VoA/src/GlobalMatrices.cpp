#include "..\include\GlobalMatrices.h"

GlobalMatrices::GlobalMatrices(void)
{
	_ModelMat = new Matrix();
	_ViewMat = new Matrix();
	_ProjMat = new Matrix();
}


GlobalMatrices::~GlobalMatrices(void)
{
	delete _ModelMat;
	delete _ViewMat;
	delete _ProjMat;
}

Matrix *GlobalMatrices::ModelMatrix()
{
	return _ModelMat;
}

Matrix *GlobalMatrices::ViewMatrix()
{
	return _ViewMat;
}

Matrix *GlobalMatrices::ProjectionMatrix()
{
	return _ProjMat;
}

void GlobalMatrices::UpdateShader()
{
	ModelMatrix()->UpdateShader("modelMat");
	ViewMatrix()->UpdateShader("viewMat");
	ProjectionMatrix()->UpdateShader("projMat");
}
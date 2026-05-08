#include "funtime_foxy.h"

FuntimeFoxy::FuntimeFoxy()
{
	position = glm::vec3(-64.94f, -3.0f, -39.62f);
	scale = glm::vec3(0.7f, 0.7f, 0.7f);
	rotationY = 0.0f;
	rotationSpeed = 0.1f;
	rotatingRight = true;
	initialized = false;
	material = Material(0.3f, 4);
}

FuntimeFoxy::~FuntimeFoxy()
{
}

void FuntimeFoxy::Initialize()
{
	if (!initialized)
	{
		foxyModel.LoadModel("Models/FuntimeFoxyOptimizado.obj");
		initialized = true;
	}
}

void FuntimeFoxy::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized)
	{
		Initialize();
	}

	// Rotación automática oscilante entre -180° y +180°
	if (rotatingRight)
	{
		rotationY += rotationSpeed;
		if (rotationY >= 120.0f)
		{
			rotationY = 120.0f;
			rotatingRight = false;
		}
	}
	else
	{
		rotationY -= rotationSpeed;
		if (rotationY <= -5.0f)
		{
			rotationY = -5.0f;
			rotatingRight = true;
		}
	}

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	foxyModel.RenderModel();
}

#include "cage_freddy.h"

CageFreddy::CageFreddy()
{
	position = glm::vec3(-14.76f, -3.0f, -70.45f);
	scale = glm::vec3(4.0f, 4.0f, 4.0f);
	rotationY = -50.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

CageFreddy::~CageFreddy()
{
}

void CageFreddy::Initialize()
{
	if (!initialized)
	{
		cageModel.LoadModel("Integrantes/Andrea/modelos/CageFreddy/CageFreddy.obj");
		initialized = true;
	}
}

void CageFreddy::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized)
	{
		Initialize();
	}

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	cageModel.RenderModel();
}

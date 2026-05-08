#include "sala_monitores.h"

SalaMonitores::SalaMonitores()
{
	position = glm::vec3(-63.77f, -3.0f, -98.08f);
	scale = glm::vec3(3.0f,3.0f, 3.0f);
	rotationY = 90.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

SalaMonitores::~SalaMonitores()
{
}

void SalaMonitores::Initialize()
{
	if (!initialized)
	{
		salaModel.LoadModel("Integrantes/Andrea/modelos/SalaMonitores/SalaMonitores.obj");
		initialized = true;
	}
}

void SalaMonitores::Render(GLuint uniformModel, GLuint uniformColor,
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

	salaModel.RenderModel();
}

#include "escenario.h"

Escenario::Escenario()
{
	position = glm::vec3(-65.35f, -3.0f, -40.79f);
	scale = glm::vec3(0.3f, 0.3f, 0.3f);
	rotationY =-120.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

Escenario::~Escenario()
{
}

void Escenario::Initialize()
{
	if (!initialized)
	{
		escenarioModel.LoadModel("Integrantes/Andrea/modelos/Escenario/EscenarioCompleto.obj");
		initialized = true;
	}
}

void Escenario::Render(GLuint uniformModel, GLuint uniformColor,
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

	escenarioModel.RenderModel();
}

#include "entrada.h"

Entrada::Entrada()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	rotationY = 90.0f;
	material = Material(0.3f, 4);
}

Entrada::Entrada(glm::vec3 pos, float rotation, glm::vec3 escala)
{
	position = pos;
	scale = escala;
	rotationY = rotation;
	material = Material(0.3f, 4);
}

Entrada::~Entrada()
{
}

void Entrada::Initialize()
{
	modelo.LoadModel("Integrantes/Andrea/modelos/Entrada/Entrada.obj");
}

void Entrada::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	glm::mat4 model(1.0);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	modelo.RenderModel();
}

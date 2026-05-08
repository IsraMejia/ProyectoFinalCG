#include "arbol1.h"

Arbol1::Arbol1()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.9f, 0.9f, 0.9f);
	rotationY = 0.0f;
	material = Material(0.3f, 4);
}

Arbol1::Arbol1(glm::vec3 pos, float rotation, glm::vec3 escala)
{
	position = pos;
	scale = escala;
	rotationY = rotation;
	material = Material(0.3f, 4);
}

Arbol1::~Arbol1()
{
}

void Arbol1::Initialize()
{
	modelo.LoadModel("Integrantes/Andrea/modelos/Arbol1/Arbol1.obj");
}

void Arbol1::Render(GLuint uniformModel, GLuint uniformColor,
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
